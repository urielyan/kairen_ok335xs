#include <QMessageBox>
#include <math.h>

#include "count_kb_value.h"
#include "ui_count_kb_value.h"
#include "calibrationmeasurement.h"
#include "global.h"
#include "printer.h"
#include  "wininforlistdialog.h"
#include "datasave.h"
#include "spectrum_painter.h"

count_kb_value::count_kb_value(QWidget *parent):
  QWidget(parent),
  ui(new Ui::count_kb_value)
{
  p_mySettings = MeasurementDataSave::instance();

  ui->setupUi(this);
  ui->comboBox->setCurrentIndex(0);
  count_record = p_mySettings->value("test").toInt();
  count_record = p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_COUNT).toInt();
  connect(this,SIGNAL(destroyed()),this,SLOT(on_pushButton_2_clicked()));
  //query_s_count_d = new query_s_count_data();
  spec_painter = new spectrum_painter();
  //printer_result();

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE * 2,QFont::Normal));
  ui->label->setObjectName("title");
}
int count_kb_value::count_record;

count_kb_value::~count_kb_value()
{
  delete spec_painter;
  //delete query_s_count_d;
  delete ui;
}

void count_kb_value::on_pushButton_2_clicked()
{
  this->close();
}

void count_kb_value::on_pushButton_clicked()
{
  if(p_mySettings->value(MYSETTINGS_CALIBRATE_COUNT).toInt() < 3){
      WinInforListDialog::instance()->showMsg(tr("标定样品数据太少"));
      return;
    }
  int count_input_s = 0;
  for(int i = 0; i < 12 ; i++){
      QString tmp_str = p_mySettings->value(MYSETTINGS_CALIBRATE_S_INPUT(i)).toString();
      qDebug() << tmp_str << i;
      if(  (!(tmp_str==NULL)) &&(!tmp_str.toDouble() == 0)  )
        count_input_s++;
    }
  qDebug() << count_input_s;
  if(count_input_s <3){
      WinInforListDialog::instance()->showMsg(tr("请输入正确的硫含量"));
      return;
    }

  if(ui->comboBox->currentText().toInt() <= 9){
      //QString kbr_data = query_s_count_d->get_kbr_a012(ui->comboBox->currentIndex() + 1);
      QString kbr_data = countKbrValue(ui->comboBox->currentIndex() + 1);

      QString kbr_blank_data = kbr_data;
      if(kbr_data == NULL){
          return;
        }
      p_mySettings->setValue(MYSETTINGS_CALIBRATE_WORK_CURVE(ui->comboBox->currentText().toInt()), kbr_data.replace(" ",";"));
      if(kbr_data == NULL)return;

      //最多只能记录20组数据，若已经到21则重新从1开始并覆盖原来的数据
      if(count_record > CALIBRATE_RESULT_MAX_RECORD) count_record = 1;


      //把计算出来的kbr值所需要的数据保存起来
      QString tmp_str;
      QMap<int,QString>painter_data;//painter data
      for(int i = 0; i <= 11; i++){
          QString tmpCalibrateInput = p_mySettings->value(MYSETTINGS_CALIBRATE_S_INPUT(i)).toString();
          QString tmpCalibrateData = p_mySettings->value(MYSETTINGS_CALIBRATE_S_DATA(i)).toString();
          //当用户未输入硫含量时，此计数不参与计算kb值,也不保存
          if(tmpCalibrateInput == NULL || !tmpCalibrateInput.compare("0.0000") || (tmpCalibrateInput.toDouble() == 0.0))continue;
          tmp_str += tmpCalibrateInput;
          tmp_str += QString("/");
          tmp_str += tmpCalibrateData;
          painter_data.insert(i, tmpCalibrateInput + "/" + tmpCalibrateData);
          tmp_str += ";";
        }
      tmp_str.chop(1);//删除最后一个分号
      QString calibrateDatakey = MYSETTINGS_CALIBRATE_RESULT_DATA(count_record);
      p_mySettings->setValue(calibrateDatakey,tmp_str);

      //保存此次计算的kbr值
      QString tmp_storeKey = MYSETTINGS_CALIBRATE_RESULT_RESULT(count_record);
      p_mySettings->setValue(tmp_storeKey,
                                                QString("%1;%2;%3;%4")
                                                .arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm"))
                                                .arg(ui->comboBox->currentText().toInt())
                                                .arg(kbr_blank_data)
                                                .arg(tmp_str.split(";").size()));


      //保存刚刚标定完成的是第几组数据再加一，以便下次计算kb值
      p_mySettings->setValue(MYSETTINGS_CALIBRATE_RESULT_COUNT,++count_record);
      WinInforListDialog::instance()->showMsg(QString("标定结果已经存入工作曲线%1").arg(ui->comboBox->currentText().toInt()));

      spec_painter->show_special_curve(ui->comboBox->currentIndex()+1,painter_data,kbr_data);
      printer_result();
      return;
  }
}

QString count_kb_value::countKbrValue(int judge_which)
{
    QStringList calibrate_data;//存储所有硫含量
    QStringList reference_proportion_wait;//标定样和参考样的比
    for(int i = 0; i <= 11 ;i++){
        //得到用户输入的硫的含量，并判断是否是空或者为0.0000
        QString input_s_data = p_mySettings->value(MYSETTINGS_CALIBRATE_S_INPUT(i)).toString();
        if(input_s_data == NULL || !input_s_data.compare("0.0000" ) || input_s_data.toDouble() == 0.0){
            continue;
        }
        calibrate_data << input_s_data;


        //得到标定的数据和带测样的数据的比值
        QStringList tmplist = p_mySettings->value(MYSETTINGS_CALIBRATE_S_DATA(i)).toString().split("/");
        if((tmplist.size() != 2)){
            WinInforListDialog::instance()->showMsg(tr("输入的硫含量样品未标定"));
            return NULL;
        }
        reference_proportion_wait << QString::number(tmplist[0].toDouble()/tmplist[1].toDouble());
    }
    //判断用户的正确输入输入了s含量，没有则提醒用户输入
    if(calibrate_data.size() < 3){
        WinInforListDialog::instance()->showMsg(tr("你需要输入硫的含量"));
    }
    double r_sum = 0;//参考样/reference的和
    double w_sum = 0;//S value的和
    double rw_sum = 0;//乘极的和
    double r_2_sum = 0;//平方的和
    double rw_sub_avrg_sum = 0;//参考样减去平均数和标定样减去平均数的乘积的和
    double r_sub_avrg_2_sum = 0;
    double w_sub_avrg_2_sum = 0;
    //double r_sum_2;//
    for(int i =0; i < calibrate_data.size() ;i++){
        r_sum += reference_proportion_wait[i].toDouble();
        w_sum += calibrate_data[i].toDouble();
        rw_sum += reference_proportion_wait[i].toDouble() * calibrate_data[i].toDouble();
        r_2_sum += pow(reference_proportion_wait[i].toDouble(),2);
    }
    double r_average = (double)r_sum/calibrate_data.size();//参考样的平均数
    double w_average = (double)w_sum/calibrate_data.size();//标定样的平均数
    for(int i = 0;i < calibrate_data.size();i++){
        rw_sub_avrg_sum += (reference_proportion_wait[i].toDouble() - r_average) * (calibrate_data[i].toDouble() - w_average);
        r_sub_avrg_2_sum += (reference_proportion_wait[i].toDouble() - r_average) * (reference_proportion_wait[i].toDouble() - r_average);
        w_sub_avrg_2_sum += (calibrate_data[i].toDouble() - w_average) * (calibrate_data[i].toDouble() - w_average);
    }
    double k = 0,b = 0,r = 0;
    if(judge_which >= 1 && judge_which <= 5){
        k = (double)(rw_sum * calibrate_data.size() - r_sum * w_sum) /(calibrate_data.size() * r_2_sum - r_sum * r_sum);
        b = (double )(w_sum * r_2_sum - r_sum * rw_sum) /(calibrate_data.size() * r_2_sum  - r_sum * r_sum);
        r = rw_sub_avrg_sum /pow(r_sub_avrg_2_sum * w_sub_avrg_2_sum,0.5);

        //存储正真用于计算的数据
        p_mySettings->setValue(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(judge_which),QString("k=%1;b=%2;r=%3").arg(k).arg(b).arg(r));

        //使得展示给用户的kbr为小数点后4位
        QString k_str = QString::number(k - (int)k);
        QString b_str = QString::number(b - (int)b);
        QString r_str  = QString::number(r - (int)r);
        if(k_str.contains("-")){
            while(k_str.size() > 7){
                k_str.chop(1);
            }
        }else {
            while(k_str.size() > 6){
                k_str.chop(1);
            }
        }

        if(b_str.contains("-")){
            while (b_str.size() > 7) {
                b_str.chop(1);
            }
        }else {
            while (b_str.size() > 6) {
                b_str.chop(1);
            }
        }

        if(r_str.contains("-")){
            while (r_str.size() > 7) {
                r_str.chop(1);
            }
        }else {
            while (r_str.size() > 6) {
                r_str.chop(1);
            }
        }

        //get k,b,r have how many number
        int k_count= 0;
        int b_count= 0;
        int r_count= 0;
        if(k_str.contains("-")){
            k_count = QString::number(k_str.toDouble()).size() - 1;
        }else {
            k_count = QString::number(k_str.toDouble()).size();
        }
        if(b_str.contains("-")){
            b_count = QString::number(b_str.toDouble()).size() - 1;
        }else {
            b_count = QString::number(b_str.toDouble()).size();
        }
        if(r_str.contains("-")){
            r_count = QString::number(r_str.toDouble()).size() - 1;
        }else {
            r_count = QString::number(r_str.toDouble()).size();
        }

        //make k,b,r totle 4 number after point
        k = (int)k + k_str.toDouble();
        k_str = QString::number(k);
        while((6 - k_count) > 0){
            k_str.append("0");
            k_count ++;
          }

        b = (int)b + b_str.toDouble();
        b_str = QString::number(b);
        while((6 - b_count) > 0){
            b_str.append("0");
            b_count ++;
          }

        r = (int)r + r_str.toDouble();
        r_str = QString::number(r);
        while((6 - r_count) > 0){
            r_str.append("0");
            r_count ++;
          }

        return QString("k=%1 b=%2 r=%3").arg(k_str).arg(b_str).arg(r_str);
    }else {
        double A0 = 0,A1 = 0,A2 = 0,A3 = 0,A4 = 0,B1 = 0,B2 = 0 ,B3 = 0;
        A0 = calibrate_data.size();
        for(int i = 0; i < A0;i++ ){
            A1 += reference_proportion_wait[i].toDouble();
            A2 += pow(reference_proportion_wait[i].toDouble(),2);
            A3 += pow(reference_proportion_wait[i].toDouble(),3);
            A4 += pow(reference_proportion_wait[i].toDouble(),4);

            B1 += calibrate_data[i].toDouble();
            B2 += calibrate_data[i].toDouble()  *  reference_proportion_wait[i].toDouble();
            B3 += pow(reference_proportion_wait[i].toDouble(),2) * calibrate_data[i].toDouble();
        }

        double a0 = ((B1 * A2 - B2 *  A1) * (A3 * A3 - A4 * A2) - (B2 * A3 - B3 * A2) * (A2 * A2 - A1 * A3)) / ((A0 * A2 - A1 * A1) *(A3 * A3 - A2 * A4) + (A1 * A3 - A2 * A2) * (A1 * A3 - A2 * A2));
        double a2 = ((B1 * A2 - B2 *  A1) * (A1 * A3 - A2 * A2) - (B2 * A3 - B3 * A2) * (A0 * A2 - A1 * A1)) / (( A1 * A1 - A0 * A2) *(A3 * A3 - A2 * A4) - (A1 * A3 - A2 * A2) * (A1 * A3 - A2 * A2));
        double a1 = (B1 - A0 * a0 - A2 * a2) / A1;

        p_mySettings->setValue(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(judge_which),
                               QString("a0=%1;a1=%2;a2=%3").arg(a0).arg(a1).arg(a2));

        QString chop_a0 = QString::number(a0 - (int)a0);
        QString chop_a1 = QString::number(a1 - (int)a1);
        QString chop_a2 = QString::number(a2 - (int)a2);

        if(chop_a0.contains("-")){
            while(chop_a0.size() > 7){
                chop_a0.chop(1);
            }
        }else {
            while(chop_a0.size() > 6){
                chop_a0.chop(1);
            }
        }


        if(chop_a1.contains("-")){
            while(chop_a1.size() > 7){
                chop_a1.chop(1);
            }
        }else {
            while(chop_a1.size() > 6){
                chop_a1.chop(1);
            }
        }

        if(chop_a2.contains("-")){
            while(chop_a2.size() > 7){
                chop_a2.chop(1);
            }
        }else {
            while(chop_a2.size() > 6){
                chop_a2.chop(1);
            }
        }

        int a0_count= 0;
        int a1_count= 0;
        int a2_count= 0;
        if(chop_a0.contains("-")){
            a0_count = QString::number(chop_a0.toDouble()).size() - 1;
        }else {
            a0_count = QString::number(chop_a0.toDouble()).size();
        }
        if(chop_a1.contains("-")){
            a1_count = QString::number(chop_a1.toDouble()).size() - 1;
        }else {
            a1_count = QString::number(chop_a1.toDouble()).size();
        }
        if(chop_a2.contains("-")){
            a2_count = QString::number(chop_a2.toDouble()).size() - 1;
        }else {
            a2_count = QString::number(chop_a2.toDouble()).size();
        }

        a0 = (int)a0 + chop_a0.toDouble();
        chop_a0 = QString::number(a0);
        while ((6 - a0_count) > 0) {
            chop_a0.append("0");
            a0_count++;
          }
        a1 = (int)a1 + chop_a1.toDouble();
        chop_a1 = QString::number(a1);
        while ((6-a1_count) > 0) {
            chop_a1.append("0");
            a1_count ++;
          }
        a2 = (int)a2 + chop_a2.toDouble();
        chop_a2 = QString::number(a2);
        while ((6-a2_count) > 0) {
            chop_a2.append("0");
            a2_count ++;
          }
        return QString("a0=%1 a1=%2 a2=%3").arg(chop_a0).arg(chop_a1).arg(chop_a2);
    }


}

void count_kb_value::printer_result(){

  int enter = 0x0A;

  long long calibrate_result = 0xFBB9E1BDA8B6EAB1;
  long long sample = 0xBFC1ACBA;
  long long calibrate = 0xF9D1A8B6EAB1;
  long long reference = 0xF9D1BCBFCEB2;

  long long work_line = 0xDFCFFAC7F7D7A4B9LL;

  printer::instance()->printEnd();

  //k=? b=? r=?
  QStringList workCurveList = p_mySettings->value(MYSETTINGS_CALIBRATE_WORK_CURVE(ui->comboBox->currentText().toInt())).toString().split(";");
  if(workCurveList.size() == 3){
      printer::transmit((void *)workCurveList[2].split("=")[0].toLocal8Bit().data(), workCurveList[2].split("=")[0].size());
      printer::transmit((void *)" =  ",3);
      printer::transmit((void *)workCurveList[2].split("=")[1].toLocal8Bit().data(), workCurveList[2].split("=")[1].size());
      printer ::transmit(enter,1);

      printer::transmit((void *)workCurveList[1].split("=")[0].toLocal8Bit().data(), workCurveList[1].split("=")[0].size());
      printer::transmit((void *)" =  ",3);
      printer::transmit((void *)workCurveList[1].split("=")[1].toLocal8Bit().data(), workCurveList[1].split("=")[1].size());
      printer ::transmit(enter,1);

      printer::transmit((void *)workCurveList[0].split("=")[0].toLocal8Bit().data(), workCurveList[0].split("=")[0].size());
      printer::transmit((void *)" =  ",3);
      printer::transmit((void *)workCurveList[0].split("=")[1].toLocal8Bit().data(), workCurveList[0].split("=")[1].size());
      printer ::transmit(enter,1);
    }

  //工作曲线：
  printer::transmit(work_line,8);
  printer::transmit((void *)"    ",3);
  printer::transmit((void *)ui->comboBox->currentText().toLocal8Bit().data(),1);
  printer ::transmit(enter,1);

  QString tmpCalibrateDataKey = QString(MYSETTINGS_CALIBRATE_RESULT_DATA( p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_COUNT).toInt() - 1) );
  QStringList datalist = p_mySettings->value(tmpCalibrateDataKey).toString().split(";");
  //所有的参与计算的硫含量和计数值。
  for(int i = datalist.size() ; i >= 1  ; i--){
      QStringList one_data = datalist[i - 1].split("/");
      if(one_data.size() != 3){
          continue;
        }
      printer::transmit(QString::number(i).toLocal8Bit().data(), QString::number(i).size());
      printer::transmit('#');
      printer::transmit((void *)"     ",2);
      printer::transmit((void *)one_data[0].toLocal8Bit().data(),one_data[0].size());
      printer::transmit((void *)"     ",4);
      printer::transmit((void *)one_data[1].toLocal8Bit().data(),one_data[1].size());
      printer::transmit((void *)"     ",4);
      printer::transmit((void *)one_data[2].toLocal8Bit().data(),one_data[2].size());
      printer ::transmit(enter,1);
    }

  //含量      标定样     参考样
  printer::transmit((void *)"            ",6);
  printer::transmit(sample,4);
  printer::transmit((void *)"       ",4);
  printer::transmit(calibrate,6);
  printer::transmit((void *)"        ",3);
  printer::transmit(reference,6);
  printer ::transmit(enter,1);


  //日期
  printer::printCurrentDateTime();

  //标点结果：
  printer::transmit(calibrate_result,8);
  printer ::transmit(enter,1);

  printer::instance()->printStart();

}
