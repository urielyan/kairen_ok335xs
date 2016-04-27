
#include "count_kb_value.h"
#include "ui_count_kb_value.h"
#include "calibrationmeasurement.h"
#include "global.h"
#include "printer.h"

#include <QMessageBox>
//extern QSettings mysettings;

count_kb_value::count_kb_value(QWidget *parent):
  QWidget(parent),
  ui(new Ui::count_kb_value)
{
  ui->setupUi(this);
  ui->comboBox->setCurrentIndex(0);
  count_record = mysettings.value("calibratemeasurement_count_record").toInt();
  connect(this,SIGNAL(destroyed()),this,SLOT(on_pushButton_2_clicked()));
  query_s_count_d = new query_s_count_data();
  spec_painter = new spectrum_painter();
  //printer_result();

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE * 2,QFont::Normal));
}
int count_kb_value::count_record;

count_kb_value::~count_kb_value()
{
  delete spec_painter;
  delete query_s_count_d;
  delete ui;
}

void count_kb_value::on_pushButton_2_clicked()
{
  this->close();
}

void count_kb_value::on_pushButton_clicked()
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  if(calibrationmeasurement::count < 3){
      msgbox.setText("标定样品数据太少");
      msgbox.exec();
      return;
    }
# if  0
  else if(calibrationmeasurement::count > 12){
      msgbox.setText("标定样品数据太多");
      msgbox.exec();
      return;
    }
#endif
  int count_input_s = 0;
  for(int i = 1; i <= calibrationmeasurement::count ; i++){
      QString tmp_str = mysettings.value(QString("calibrate_input_s_%1").arg(i)).toString();
      if(  (!(tmp_str==NULL)) &&(!tmp_str.toDouble() == 0.0)  )
        count_input_s++;
    }
  if(count_input_s <3){
      msgbox.setText("请输入正确的硫含量");
      msgbox.exec();
      return;
    }

  if(ui->comboBox->currentText().toInt() <= 9){
      QString kbr_data = query_s_count_d->get_kbr_a012(ui->comboBox->currentIndex() + 1);
      QString kbr_blank_data = kbr_data;
      if(kbr_data == NULL){
          return;
        }
      mysettings.setValue(QString("work_curve_%1").arg(ui->comboBox->currentText().toInt()),kbr_data.replace(" ",";"));
      if(kbr_data == NULL)return;
      msgbox.setText(kbr_blank_data);

      //最多只能记录20组数据，若已经到21则重新从1开始并覆盖原来的数据
      if(count_record > CALIBRATE_RESULT_MAX_RECORD) count_record = 1;


      //把计算出来的kbr值所需要的数据保存起来
      QString tmp_str;
      QMap<int,QString>painter_data;//painter data
      for(int i = 1; i <= calibrationmeasurement::count ; i++){
          QString tmp_s_samplement = mysettings.value(QString("calibrate_input_s_%1").arg(i)).toString();
          //当用户未输入硫含量时，此计数不参与计算kb值,也不保存
          if(tmp_s_samplement == NULL || !tmp_s_samplement.compare("0.0000") || (tmp_s_samplement.toDouble() == 0.0))continue;
          tmp_str += tmp_s_samplement;
          tmp_str += QString("/");
          tmp_str += mysettings.value(QString("s_count_data_%1").arg(i)).toString();
          painter_data.insert(i,tmp_s_samplement + "/" + mysettings.value(QString("s_count_data_%1").arg(i)).toString());
          tmp_str += ";";
        }
      tmp_str.chop(1);//删除最后一个分号
      mysettings.setValue(QString("calibration_results_in_data_%1").arg(count_record),tmp_str);

      //保存此次计算的kbr值
      mysettings.setValue(QString("calibration_results_in_result_%1").arg(count_record),\
                          QString("%1;%2;%3;%4")\
                          .arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm"))\
                          .arg(ui->comboBox->currentText().toInt())\
                          .arg(kbr_blank_data)\
                          .arg(tmp_str.split(";").size()));


      //保存刚刚标定完成的是第几组数据再加一，以便下次计算kb值
      mysettings.setValue("calibratemeasurement_count_record",++count_record);
      msgbox.setInformativeText(QString("标定结果已经存入工作曲线%1").arg(ui->comboBox->currentText().toInt()));
      msgbox.exec();

      spec_painter->show_special_curve(ui->comboBox->currentIndex()+1,painter_data,kbr_data);
      printer_result();
      return;
    }
}

void count_kb_value::printer_result(){

  int enter = 0x0A;

  long long calibrate_result = 0xFBB9E1BDA8B6EAB1;
  long long sample = 0xBFC1ACBA;
  long long calibrate = 0xF9D1A8B6EAB1;
  long long reference = 0xF9D1BCBFCEB2;

  long long work_line = 0xDFCFFAC7F7D7A4B9LL;

  printer::instance()->printStart();

#if 1
  printer::transmit(calibrate_result,8);
  printer ::transmit(enter,1);

  //日期
  printer::printCurrentDateTime();

  printer::transmit((void *)"            ",5);
  printer::transmit(sample,4);
  printer::transmit((void *)"       ",5);
  printer::transmit(calibrate,6);
  printer::transmit((void *)"     ",2);
  printer::transmit(reference,6);
  printer ::transmit(enter,1);

  QStringList datalist = mysettings.value(QString("calibration_results_in_data_%1")\
                                          .arg(mysettings.value("calibratemeasurement_count_record").toInt() - 1)).toString().split(";");
  qDebug() << datalist;
  for(int i = 1 ; i <= datalist.size() ; i++){
      QStringList one_data = datalist[i - 1].split("/");
      if(one_data.size() != 3){
          continue;
        }
      printer::transmit((char)(i + 0x30));
      printer::transmit('#');
      printer::transmit((void *)"     ",2);
      printer::transmit((void *)one_data[0].toLocal8Bit().data(),one_data[0].size());
      printer::transmit((void *)"     ",4);
      printer::transmit((void *)one_data[1].toLocal8Bit().data(),one_data[1].size());
      printer::transmit((void *)"     ",4);
      printer::transmit((void *)one_data[2].toLocal8Bit().data(),one_data[2].size());
      printer ::transmit(enter,1);
    }

  printer::transmit(work_line,8);
  printer::transmit((void *)"    ",3);
  printer::transmit((void *)ui->comboBox->currentText().toLocal8Bit().data(),1);
  printer ::transmit(enter,1);

#endif
  QStringList work_curve_list = mysettings.value(QString("work_curve_%1").arg(ui->comboBox->currentText())).toString().split(";");
  if(work_curve_list.size() == 3){
      printer::transmit((void *)work_curve_list[0].split("=")[0].toLocal8Bit().data(),work_curve_list[0].split("=")[0].size());
      printer::transmit((void *)"   ",3);
      printer::transmit((void *)work_curve_list[0].split("=")[1].toLocal8Bit().data(),work_curve_list[0].split("=")[1].size());
      printer ::transmit(enter,1);

      printer::transmit((void *)work_curve_list[1].split("=")[0].toLocal8Bit().data(),work_curve_list[1].split("=")[0].size());
      printer::transmit((void *)"   ",3);
      printer::transmit((void *)work_curve_list[1].split("=")[1].toLocal8Bit().data(),work_curve_list[1].split("=")[1].size());
      printer ::transmit(enter,1);
      printer::transmit((void *)work_curve_list[2].split("=")[0].toLocal8Bit().data(),work_curve_list[2].split("=")[0].size());
      printer::transmit((void *)"   ",3);
      printer::transmit((void *)work_curve_list[2].split("=")[1].toLocal8Bit().data(),work_curve_list[2].split("=")[1].size());
      printer ::transmit(enter,1);
    }
  printer::instance()->printEnd();
}
