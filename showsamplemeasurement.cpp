#include "showsamplemeasurement.h"
#include "ui_showsamplemeasurement.h"
#include "input_person_sampleserial.h"
#include "global.h"
#include "printer.h"

#include <stdio.h>
#include <countingmeasurement.h>
#include <math.h>
#include <QScrollBar>
#include <QSqlError>
#include <QSqlDriver>


showsamplemeasurement::showsamplemeasurement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showsamplemeasurement)
{
    ui->setupUi(this);
    sum = 0;
    real_curve = 0;
    model = new QSqlTableModel();

    hide_lable(true);
    ui->tableWidget->setColumnWidth(0,this->width()/2);
    ui->tableWidget->setColumnWidth(1,this->width()/2);

    QScrollBar *verticalbar;
    verticalbar = new QScrollBar();
    verticalbar->setStyleSheet("width:30px");
    ui->tableWidget->setVerticalScrollBar(verticalbar);

    ui->tableWidget_hide->hide();

    //printer_result();
    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      }
    ui->pushButton->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
    ui->label->setObjectName("title");
}

showsamplemeasurement::~showsamplemeasurement()
{
    delete ui;
    delete model;
}

void showsamplemeasurement::add_data(int work_curve_index,QString data,int size){
    if(1 == size) {
        ui->tableWidget->clear();
        ui->tableWidget_hide->clear();
        ui->tableWidget_hide->setRowCount(COUNT_MEASUREMENT_DATA_COUNT);
        for(int i = 0; i < COUNT_MEASUREMENT_DATA_COUNT; i++ ){
            ui->tableWidget_hide->setItem(i,1,\
              new QTableWidgetItem(countingMeasurement::get_count_5_data().value(i)));
          }
        hide_lable(true);
        sum = 0;
    }
    //得到5个样品的平均数
    double sample_average = countingMeasurement::get_count_5_average();
    //ui->label->setText(QString("sample_average:%1").arg(sample_average));
    if(sample_average == 0){
        return;
    }

    //判断是否是自动选择工作曲线,若是，则选择一个合适的工作曲线
    if(work_curve_index == 0){
        double r = (data.toDouble())/sample_average;
#ifdef DEBUG
    QMessageBox::warning(this,QString(__FILE__),QString(__FUNCTION__) + QString(":%1,%2,%3").arg(data.toDouble()).arg(sample_average).arg(r));
#endif
        if(mysettings.contains("proportion_1") && mysettings.contains("proportion_2")){
#ifdef DEBUG
            QMessageBox::warning(this,QString(__FILE__),QString("contain proportion"));
#endif
            if(r < mysettings.value("proportion_1").toDouble()){
                work_curve_index = 1;
            }else if(r >= mysettings.value("proportion_1").toDouble() && r < mysettings.value("proportion_2").toDouble()){
                work_curve_index = 2;
            }else if(r >= mysettings.value("proportion_2").toDouble()){
                work_curve_index = 6;
            }
        }else {
            if(r < 0.1){
                work_curve_index = 1;
            }else if(r >= 0.1 && r < 1.0){
                work_curve_index = 2;
            }else if(r >= 1.0){
                work_curve_index = 6;
            }
        }
    }
    real_curve = work_curve_index;

    //得到kb值
    QString work_curve_value = mysettings.value("real_compute_kbr_" + QString::number(work_curve_index)).toString();
    QStringList work_curve_list;
    double percentage = 0;
    work_curve_list = work_curve_value.split(";");
    if(work_curve_index <= 5 && work_curve_index >= 1){
        if((work_curve_list.size() != 3) || (work_curve_list[0] == NULL)|| (work_curve_list[1] == NULL))return;
        if(work_curve_list[0].split("=").size() == 2){
            percentage = work_curve_list[0].split("=")[1].toDouble() * \
                    ((data.toDouble())/sample_average) + work_curve_list[1].split("=")[1].toDouble();
        }
    }else if((work_curve_index > 5) && (work_curve_index <= 9)){
        if((work_curve_list.size() != 3) || (work_curve_list[0] == NULL)|| \
                (work_curve_list[1] == NULL)|| (work_curve_list[2] == NULL))return;
        if(work_curve_list[0].split("=").size() == 2){
            percentage = work_curve_list[0].split("=")[1].toDouble() + \
                    work_curve_list[1].split("=")[1].toDouble() * ((data.toDouble())/sample_average) + \
                    work_curve_list[2].split("=")[1].toDouble()* pow(((data.toDouble())/sample_average),2);
        }
    }else{
        percentage = 0;
       return;
    }

    if(percentage < 0.0){
        percentage = 0.0;
      }

    //let percentage have 4 number after point
    QString percentage_str = QString::number(percentage,'f',4);
    ui->tableWidget->setRowCount(size);
    for(int i = 0;i < size;i++){
        ui->tableWidget->setRowHeight(i,FONT_SIZE*3/2);
      }
    ui->tableWidget->setRowHeight(size,FONT_SIZE*3/2);
    if(size <= COUNT_MEASUREMENT_DATA_COUNT){
        ui->tableWidget_hide->setRowCount(COUNT_MEASUREMENT_DATA_COUNT);
      }else{
        ui->tableWidget_hide->setRowCount(size);
      }
    ui->tableWidget_hide->setItem(size -1,0,new QTableWidgetItem(data));
    ui->tableWidget->setItem(size-1,0,new QTableWidgetItem(QString("第 %1 次测量").arg(size)));
    if(percentage == 0.0){
        ui->tableWidget->setItem(size-1,1,new QTableWidgetItem(QString("0.0001")));
      }else{
        while (percentage_str.size() < 6) {
            percentage_str.append("0");
          }
        if(percentage_str.toDouble() > 100)percentage_str = "99.9999";
        ui->tableWidget->setItem(size-1,1,new QTableWidgetItem(percentage_str));
      }
    sum += ui->tableWidget->item(size -1,1)->text().toDouble();
}

void showsamplemeasurement::show_calculate_storage(QString data){
    if(data == NULL)return;
    int rows = ui->tableWidget->rowCount();
    hide_lable(false);

    //get and storage average
    //double average = sum / rows - (int)(sum / rows);
    double average = sum / rows;
    QString str_average = QString::number(average,'f',4);

    while(str_average.size() < 6){
        str_average.append("0");
      }
    ui->label_average->setText(str_average);
    data += str_average;
    data += ";";


    double standard_deviation = 0.0;
    double n_avg_sub_2_sum = 0.0;

    for(int i = 0;i < rows;i++){
        n_avg_sub_2_sum += pow(sum / rows - ui->tableWidget->item(i,1)->text().toDouble(),2);
    }
    standard_deviation = pow(n_avg_sub_2_sum/(rows - 1),0.5);

    QString str_deviation = QString::number(standard_deviation,'f',4);

    while (str_deviation.size() < 6) {
        str_deviation.append("0");
    }
    ui->label_deviation->setText(str_deviation);
    data += str_deviation;
    int tmp = mysettings.value("sample_count").toInt();
    tmp++;

    //存储数据到文本文件中。
    //mysettings.setValue(QString("sample_data_%1").arg(tmp),data);
    mysettings.setValue("sample_count",tmp);

    //存贮数据到数据库中：
    bool ok;
    QStringList data_list = data.split(";");
    //db.transaction();
#if 1
    QSqlQuery query;
    query.prepare("INSERT INTO sample_data (people_id ,sample_serial, date_time,work_curve,measurement_time,repeat_time,average,deviation,is_auto,current_coefficient) "
                  "VALUES (?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(input_person_sampleSerial::line_people);
    query.addBindValue(input_person_sampleSerial::line_serial);
    query.addBindValue(data_list[1]);
    query.addBindValue(real_curve);
    query.addBindValue(data_list[2]);
    query.addBindValue(data_list[3]);
    query.addBindValue(data_list[4]);
    query.addBindValue(data_list[5]);
    query.addBindValue(data_list[0].toInt() == 0 ? "是" : "否");
    query.addBindValue(mysettings.value("work_curve_" + QString::number(real_curve)).toString());
    ok  = query.exec();
    if(ok == false){
        QMessageBox box;
        box.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
        box.setText(tr("数据未存入数据库中！"));
        box.exec();
    }
    query.finish();
#endif
//    ok = db.commit();
//    if(ok == false ){
//        QMessageBox box;
//        box.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
//        box.setText(db.lastError().text() + QString::number(db.driver()->hasFeature(QSqlDriver::Transactions)));
//        box.exec();
//    }
    printer_result();
    this->showFullScreen();
}

void showsamplemeasurement::hide_lable(bool judge){
  return;
    if(judge == false){
        ui->label->showFullScreen();
        ui->label_2->showFullScreen();
        ui->label_3->showFullScreen();
        ui->label_4->showFullScreen();
        ui->label_deviation->showFullScreen();
        ui->label_average->showFullScreen();

    }else {
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_deviation->hide();
        ui->label_average->hide();
    }
}

void showsamplemeasurement::on_pushButton_clicked()
{
    this->close();
  ui->tableWidget_hide->hide();
}

void showsamplemeasurement::clear_tablewidget(){
    ui->tableWidget->clear();
    ui->tableWidget_hide->clear();
    hide_lable(true);
}

void showsamplemeasurement::printer_result(){
  //QMessageBox::warning(this,"showsample","printer");

  int enter = 0x0A;
  long long average  = 0xB5D6F9BEBDC6ll;
  long long deviation = 0xEEB2ABC6BCD7EAB1ll;
  int di = 0xDAB5;
  long long ciceliang = 0xBFC1E2B2CEB4;
  printer::transmit((void *)"---------------------------------------------------------",SEGMENT_LENGTH );
  printer ::transmit(enter,1);
  for(int i = 0 ;i < ui->tableWidget->rowCount();i++){
      printer::transmit(di,2);
      printer::transmit((char)(i + 0x30 + 1));
      printer::transmit(ciceliang,6);
      printer::transmit((void *)"   ",3);

      QString item_data = ui->tableWidget->item(i,1)->text();
      if(item_data == NULL){
          break;
        }
      printer::transmit((void *)item_data.toLocal8Bit().data(),item_data.size());
      printer::transmit((void *)"   ",3);
      printer::transmit((void *)"%(m/m)",6);
      printer ::transmit(enter,1);
    }
  printer::transmit((void *)"---------------------------------------------------",SEGMENT_LENGTH );
  printer ::transmit(enter,1);


  printer::transmit(average,6);
  printer::transmit((void *)":      ",7);
  printer::transmit((void *)ui->label_average->text().toLocal8Bit().data(),ui->label_average->text().size());
  printer::transmit((void *)"   ",3);
  printer::transmit((void *)"%(m/m)",6);
  printer ::transmit(enter,1);

  printer::transmit(deviation,8);
  printer::transmit((void *)":    ",5);
  printer::transmit((void *)ui->label_deviation->text().toLocal8Bit().data(),ui->label_deviation->text().size());
  printer::transmit((void *)"   ",3);
  printer::transmit((void *)"%(m/m)",6);
  printer ::transmit(enter,1);

  printer::transmit((void *)"========================================",SEGMENT_LENGTH);
  printer ::transmit(enter,1);
  printer::transmit((void *)"   ",3);
  printer ::transmit(enter,1);
}

int showsamplemeasurement::get_real_curve(){
  return real_curve;
}

void showsamplemeasurement::on_pushButton_2_clicked()
{
    ui->tableWidget_hide->show();
}
