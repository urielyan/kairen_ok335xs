#include "query_s_count_data.h"
#include "ui_query_s_count_data.h"
#include "global.h"

#include <QDebug>
#include <calibrationmeasurement.h>
#include <math.h>
#include <QMessageBox>

//extern QSettings mysettings;

query_s_count_data::query_s_count_data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::query_s_count_data)
{
    ui->setupUi(this);
    int row= 0 , column = 0;

    ui->tableWidget->setColumnWidth(0,FONT_SIZE *3);
    ui->tableWidget->setColumnWidth(1,FONT_SIZE *10);
    ui->tableWidget->setColumnWidth(2,FONT_SIZE *3);
    ui->tableWidget->setColumnWidth(3,FONT_SIZE *10);
    for(int i = 0;i < 6; i++){
        ui->tableWidget->setRowHeight(i,FONT_SIZE * 1.5);
    }
    for(int tmpnumber = 0 ; tmpnumber <12 ; tmpnumber++){
        tmpitem[tmpnumber] = new QTableWidgetItem();
    }
    for(column = 1 ; column < 4 ; column += 2){
        for(row = 0 ; row < 6 ; row++){
            int tmp = (1 == column) ? (row + column) : (row + column +4);
            QString tmpsettingsstr = QString("s_count_data_%1").arg(tmp);
            QString tmpsettings = mysettings.value(tmpsettingsstr).toString();
            tmpitem[tmp - 1]->setText(tmpsettings);
            ui->tableWidget->setItem(row,column,tmpitem[tmp - 1]);
        }
    }
    //printf("%s\n",__FUNCTION__);
    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      }
    ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));
    ui->tableWidget->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    ui->label->setObjectName("title");
}

query_s_count_data::~query_s_count_data()
{
    delete ui;
}


//得到以字符串形式的kbr
QString query_s_count_data::get_kbr_a012(int judge_which){
    int m = mysettings.value("calibratemeasurement_count").toInt();
    QStringList calibrate_data;//存储所有硫含量
    QStringList reference_proportion_wait;//标定样和参考样的比
    for(int i = 0; i < m ;i++){
        //得到用户输入的硫的含量，并判断是否是空或者为0.0000
        QString input_s_data = mysettings.value( QString("calibrate_input_s_%1").arg(i)).toString();
        if(input_s_data == NULL || !input_s_data.compare("0.0000" ) || input_s_data.toDouble() == 0.0){
            continue;
        }
        calibrate_data << input_s_data;


        //得到标定的数据和带测样的数据的比值
        QStringList tmplist = mysettings.value(QString("s_count_data_%1").arg(i + 1)).toString().split("/");
        if((tmplist.size() != 2)){
            return NULL;
        }
        reference_proportion_wait << QString::number(tmplist[0].toDouble()/tmplist[1].toDouble());
    }
    //判断用户的正确输入输入了s含量，没有则提醒用户输入
    if(calibrate_data.size() < 3){
        QMessageBox msgbox;
        msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
        msgbox.setText("你需要输入硫的含量");
        msgbox.exec();
        return NULL;
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
        mysettings.setValue(QString("real_compute_kbr_%1").arg(judge_which),QString("k=%1;b=%2;r=%3").arg(k).arg(b).arg(r));

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

        mysettings.setValue(QString("real_compute_kbr_%1").arg(judge_which),QString("a0=%1;a1=%2;a2=%3").arg(a0).arg(a1).arg(a2));

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



void query_s_count_data::show_and_refresh(){
    int column ,row;
    for(column = 1 ; column < 4 ; column += 2){
        for(row = 0 ; row < 6 ; row++){
            int tmp = (1 == column) ? (row + column) : (row + column +4);
            QString tmpsettingsstr = QString("s_count_data_%1").arg(tmp);
            QString tmpsettings = mysettings.value(tmpsettingsstr).toString();
            tmpitem[tmp - 1]->setText(tmpsettings);
            //ui->tableWidget->setItem(row,column,tmpitem[tmp - 1]);
        }
    }

    this->showFullScreen();
}

void query_s_count_data::add_s_count_data(int i ,QString str_data){
    if(i > 12)return;//judge whether count greater than 12,if yes don't store to mysetting.
    mysettings.setValue(QString("s_count_data_%1").arg(i),str_data);
}

void query_s_count_data::clear_data(){
    int tmpnumber,row,column;
    QString tmpstr = "s_count_data_";
    for(tmpnumber = 1; tmpnumber <= 12;tmpnumber++){
        tmpstr.append(QString("%1").arg(tmpnumber));
        //qDebug() << tmpstr;
        mysettings.setValue(tmpstr,"");
        tmpstr = "s_count_data_";
    }
    for(row = 0;row < 6;row++)
        for(column = 1 ;column < 4 ; column += 2){
            ui->tableWidget->item(row,column)->setText("");
        }

}
void query_s_count_data::on_pushButton_clicked()
{
    this->close();
}

void query_s_count_data::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug() << index;
}
