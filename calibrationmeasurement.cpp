
#include "calibrationmeasurement.h"
#include "ui_calibrationmeasurement.h"
#include "global.h"

#include <QMessageBox>

//extern QSettings mysettings;
extern int measurement_flag;

#define CALIBRATE_TIME 90
calibrationmeasurement::calibrationmeasurement(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::calibrationmeasurement)
{
  ui->setupUi(this);

  second = CALIBRATE_TIME;
  /*
      count_flag = 1 ,It will measure reference.
    */
  count_flag = 0;
  /*
        button_flag = 1,It's doing measurement.
        button_flag = 2,It's not doing measurement.
    */
  button_flag= 0;

  ui->label_issamlpe->hide();
  ui->label_2->setText(QString("测量时间2×%1秒").arg(CALIBRATE_TIME));

  //judge calibratemeasurement_count  whether is exit;if not exit create it.finally assign count variable.
  if(!mysettings.contains("calibratemeasurement_count")){
      mysettings.setValue("calibratemeasurement_count",0);
    }
  count = mysettings.value("calibratemeasurement_count").toInt();

  showcalibratemeasure = new showcalibratemeasurement();
  query_s_count_data_in_this = new query_s_count_data();
  timer =  new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(doing_measurement()));

  calibrate_com =  Communciation_Com::instance();

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
  ui->label_second->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));

  ui->pushButton_2->setObjectName("stop");
  ui->pushButton->setObjectName("start");
  this->setStyleSheet(""
                      "QPushButton#stop{background-color:red; color: rgb(255, 255, 255);font-weight:bold;}"
                      "QPushButton#start{background-color:green; color: rgb(255, 255, 255);font-weight:bold;}");
}
int calibrationmeasurement::count;

calibrationmeasurement::~calibrationmeasurement()
{
  //mysettings.setValue("calibratemeasurement_count",count);
  delete calibrate_com;
  delete query_s_count_data_in_this;
  delete showcalibratemeasure;
  delete timer;
  delete ui;
}

void calibrationmeasurement::doing_measurement(){
  //timer slot
  ui->label_issamlpe->show();
  static QString query_data;
  QString tmpstr_second,tmpstr_count;
  tmpstr_count = QString("标定样序号 %1").arg(count);
  tmpstr_second =QString("%1 秒").arg(second);
  ui->label_number->setText(tmpstr_count);
  ui->label_second->setText(tmpstr_second);

  if(second == 0){
      QString recv_data = Communciation_Com::receive(5);
      //ui->label->setText(recv_data.toLocal8Bit().toHex());//test
      if(recv_data == NULL){
          QSettings communication_err_data("shanghaikairen","communication_error");
          communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);
          count--;
          QMessageBox msgbox;
          msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          msgbox.setText("未接收到标定数据，已停止测量");
          on_pushButton_2_clicked();//stop button
          msgbox.exec();
          return;
        }
#if 1
      if(recv_data[0] != (char)0x04){
          QSettings communication_err_data("shanghaikairen","communication_error");
          communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);
          count--;
          QMessageBox msgbox;
          msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          msgbox.setText(QString("标定数据有误，已停止测量"));
          on_pushButton_2_clicked();//stop button
          msgbox.exec();
          return;
        }
#endif
      //检查是否为带测样数据,若是则改变滑板状态
      if(recv_data[3] == (char)0x32 && recv_data.size() == 9){//count_flag == 0 &&
          recv_data.remove(0,4);
          showcalibratemeasure->add_calibratemeasurement_data(count,recv_data,NULL);
          query_data = recv_data;
          query_data += "/";

          //等待滑板改变
          //sleep(SLIDING_PLATE_CHANGE_TIME - 3);
          recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
          if(recv_data == NULL){
              recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
              if(recv_data == NULL){
                  QSettings communication_err_data("shanghaikairen","communication_error");
                  communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);
                  count--;
                  QMessageBox msgbox;
                  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
                  msgbox.setText("err:no receive data!");
                  on_pushButton_2_clicked();//stop button
                  msgbox.exec();
                  return;}
            }
          if( recv_data[1] == (char)0x33){//recv_data[0] == (char)0x98 &&
              count --;
              on_pushButton_2_clicked();

              QMessageBox msgbox;
              msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
              msgbox.setText(MACHINE_MALFUNCTION_TEXT);
              msgbox.exec();
              return;
            }else if(recv_data[1] == (char)0x31){//recv_data[0] == (char)0x98 &&
              //count_flag = 1;//说明已接收到待测样数据，开始测量参考样
              second = CALIBRATE_TIME;
              ui->label_issamlpe->setText(REFERENCE_BE_LOCATON_TEXT);
              ui->widget->change_label_content(REFERENCE_BE_LOCATON);
              return;
            } else{
              QSettings communication_err_data("shanghaikairen","communication_error");
              communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);

              count--;
              QMessageBox msgbox;
              msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
              msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT+recv_data);
              on_pushButton_2_clicked();//stop button
              msgbox.exec();
              return;
            }
        }else if(recv_data[3] == (char)0x31 && recv_data.size() == 9){//检查是否为参考样数据1 == count_flag &&
          //measurement sample
          recv_data.remove(0,4);
          query_data += recv_data;
          ui->label_second->clear();
          ui->label_number->clear();
          ui->label_issamlpe->clear();
          button_flag = 0;
          //count_flag = 0;
          second = CALIBRATE_TIME;
          timer->stop();
          disable_button(false);
          measurement_flag = MEASUREMENT_NOTHING;
          showcalibratemeasure->add_calibratemeasurement_data(count,NULL,recv_data);
          showcalibratemeasure->showFullScreen();
          query_s_count_data_in_this->add_s_count_data(count,query_data);
          mysettings.setValue("calibratemeasurement_count",count);
          return;
        }else{
          QSettings communication_err_data("shanghaikairen","communication_error");
          communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);

          QMessageBox msgbox;
          msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          //check preheat condition
          msgbox.setText("数据不合法1!\n" + recv_data);
          msgbox.exec();
          on_pushButton_2_clicked();
          return;
        }
    }else if (second < 0){
      second = 0;
    }
  second--;
}

void calibrationmeasurement::on_pushButton_clicked()
{
#if 1
  //test
  ui->pushButton->setDisabled(true);
  ui->pushButton_2->setDisabled(true);
  ui->pushButton_3->setDisabled(true);
  ui->pushButton_4->setDisabled(true);
#endif
  //start button
  if(1 == button_flag)return;
  if(ui->widget_2->global_ispreheat > 0){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      //check preheat condition
      msgbox.setText("你的仪器需要预热，请查看下方!");
      msgbox.exec();
      disable_button(false);
      return;
    }
  count = mysettings.value("calibratemeasurement_count").toInt();
  if(count >= 12){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      //check preheat condition
      msgbox.setText("标定样已超过12个");
      msgbox.exec();
      disable_button(false);
      return;
    }
  //judge whether have any other measurement.
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
//开始之前停止任何测量
  if(on_pushButton_2_clicked() != ALL_RIGHT)return;

  //开始标定测量
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  usleep(100);
  if(Communciation_Com::transmit(ACTIVATING_CALIBRATE,3) <= 0){
      measurement_flag = MEASUREMENT_NOTHING;
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.exec();
      disable_button(false);
      return;
    }
  QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
  if(recv_data == NULL){
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
      msgbox.setInformativeText("recv NULL");
      msgbox.exec();
      disable_button(false);
      return;
    }
  if( recv_data[1] == (char)0x33){//recv_data[0] == (char)0x98 &&
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(MACHINE_MALFUNCTION_TEXT);
      on_pushButton_2_clicked();
      msgbox.exec();
      measurement_flag = MEASUREMENT_NOTHING;
      ui->widget->global_is_sample = REFERENCE_BE_LOCATON;
      disable_button(false);
      return;
    }else if(recv_data[1] == (char)0x32){//recv_data[0] != (char)0x98 ||
      //startig
      disable_button(true);
      button_flag = 1;
      ui->widget->change_label_content(WAIT_BE_LOCATION);
      showcalibratemeasure->clear_label();
      ui->label_issamlpe->setText(WAIT_BE_LOCATION_TEXT);
      timer->start(1000);
      measurement_flag = MEASUREMENT_CALIBRATE;
      tcflush(Communciation_Com::fd,TCIOFLUSH);
      count++;
      //started
      return;
    }else{
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_4",communication_err_data.value("com_err_4").toInt() + 1);
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
      on_pushButton_2_clicked();
      msgbox.exec();
    }
}

void calibrationmeasurement::on_pushButton_4_clicked()
{
  //query button
  showcalibratemeasure->add_calibratemeasurement_data(count,NULL,NULL);
  showcalibratemeasure->showFullScreen();
}

int  calibrationmeasurement::on_pushButton_2_clicked()
{
  //stop button
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  ui->label_second->clear();
  ui->label_number->clear();
  ui->label_issamlpe->clear();
  second = CALIBRATE_TIME;
  button_flag = 2;
  timer->stop();
  measurement_flag = MEASUREMENT_NOTHING;
  disable_button(false);
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  if(Communciation_Com::transmit(STOP_ORDER,3) <0){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.exec();
      return ERRNO_COMMUNICATION_1;
    }
  usleep(100);
  QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
  if(recv_data == NULL){
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_10",communication_err_data.value("com_err_10").toInt() + 1);
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
      msgbox.setInformativeText("recv NULL");
      msgbox.exec();
      return ERRNO_COMMUNICATION_1;
    }
  if(recv_data[1] == (char)0x31){//recv_data[0] == (char)0x98 &&
      ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    }else if(recv_data[1] == (char)0x32){
      ui->widget->change_label_content(WAIT_BE_LOCATION);
    }else if(recv_data[1] == (char)0x33){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(MACHINE_MALFUNCTION_TEXT);
      msgbox.exec();
      return ERRNO_SILIDING_POSITION;
    }else{
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_10",communication_err_data.value("com_err_10").toInt() + 1);
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(QString("通信不正常") + ":" + recv_data + "," + QString::number(measurement_flag));
      msgbox.exec();
      return ERRNO_COMMUNICATION_1;
    }
  return ALL_RIGHT;
}

void calibrationmeasurement::on_pushButton_3_clicked()
{
  this->close();
}

void calibrationmeasurement::disable_button(bool value){
  ui->pushButton->setDisabled(value);
  ui->pushButton_3->setDisabled(value);
  ui->pushButton_2->setDisabled(false);
  ui->pushButton_4->setDisabled(false);
}
