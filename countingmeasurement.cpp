#include <QMessageBox>

#include <stdio.h>
#include <unistd.h>

#include "countingmeasurement.h"
#include "ui_countingmeasurement.h"
#include "showcountingmeasurement.h"
#include "global.h"
#include "printer.h"
#include "datasave.h"
#include "com.h"
#include "wininforlistdialog.h"

extern int measurement_flag;

countingMeasurement::countingMeasurement(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::countingMeasurement)
{
    p_mySettings = MeasurementDataSave::instance();

  ui->setupUi(this);
  show_count_mea = new showcountingmeasurement();
  turn_counter = 1;//
  turn_second = TURN_SECONDS;

  //while flag = 1,表示没有进行计数测量.
  //while flag = 2,表示正在进行计数测量.
  flag = 0;

  try_inspectoscope_flag = 0;
  ui->label_2->setText(QString("共11次,每次%1秒").arg(TURN_SECONDS));
  ui->label_seconds->setText(QString("%1秒").arg(TURN_SECONDS));

  timer = new QTimer();
  connect(timer,SIGNAL(timeout()),this,SLOT(count_second()));

    set_sliding_disabled(true,false);

    ui->pushButton_2->setObjectName("stop");
    ui->pushButton->setObjectName("start");
    this->setStyleSheet(""
                        "QPushButton#stop{background-color:rgb(255, 0, 0); color: rgb(255, 255, 255);font-weight:bold;}"
                        "QPushButton#start{background-color:rgb(0, 255, 0); color: rgb(255, 255, 255);font-weight:bold;}");
    ui->label_seconds->setObjectName("countDown");
    ui->pushButton_7->setObjectName("buttonTitle");

    INIT_LABEL_SIZE_FONT;
     ui->label_seconds->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));
     ui->pushButton_7->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));

#ifdef FRIENDLYARM_TINY210
     QList<QPushButton *> btnList = this->findChildren<QPushButton *>();
     btnList.removeAt(btnList.indexOf(ui->pushButton_7));
     for (int i = 0; i < btnList.count(); ++i) {
         btnList[i]->setFixedHeight(DESKTOP_HEIGHT / 7);
       }
#endif
}
countingMeasurement::~countingMeasurement()
{
  delete timer;
  delete show_count_mea;
  delete ui;
}

/*下面四个函数是为了含量测量而设计的最新5个计数的平均值
 *  count_data_5:一个以时间排序的QMap对象，一个时间对应一个计数值，最多5个
    第一个函数：实现了更新计数数据，保证5个数据是最新的
    第二个函数：一个静态公有函数，得到5个最新计数值的平均值。当计数值的时间超过规定时间，则返回0
    第三个函数：开始计数测量，开始之前不发送停止信号。
    第四个函数：是一个静态公有槽函数，检查5个数据是否有效,若无效，则进行计数测量并返回-1，否则返回0，这个槽函数要每分钟进行一次。若正在进行计数测量，则不做任何事。
*/
static QMap<QDateTime,int> countData5;
void countingMeasurement::updata_count_5_average(QDateTime count_datetime,int data)
{
  if(countData5.size() < COUNT_MEASUREMENT_DATA_COUNT){
      countData5.insert(count_datetime,data);
      return;
    }else {
      countData5.remove(countData5.begin().key());
      countData5.insert(count_datetime,data);
    }

  if(measurement_flag == MEASUREMENT_10_AUTO && turn_counter == COUNT_MEASUREMENT_DATA_COUNT){
      //printf("ready\n");
      static int in_this_cunction_time = 0;
      in_this_cunction_time ++;
      int tmp_count = p_mySettings->value(MYSETTINGS_COUNT_COUNT).toInt() + 1;
      if(tmp_count > COUNT_MEASUREMENT_MOST_STORAGE) tmp_count = 1;

      if(in_this_cunction_time  < 2){
          //printf("no\n");
          return;
        }
      in_this_cunction_time = 0;
      p_mySettings->setValue(MYSETTINGS_COUNT_DATA(tmp_count),
                             QString("%1").arg((int)get_count_5_average())
                          + ";" + "  " + ";" + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
      p_mySettings->setValue(MYSETTINGS_COUNT_COUNT,tmp_count);
    }
}
double countingMeasurement::get_count_5_average(){
  double average = 0;
  QMap<QDateTime,int>::iterator i;
  for(i = countData5.begin();i != countData5.end();i++){
      average += i.value();
    }
  average /= (double)COUNT_MEASUREMENT_DATA_COUNT;
  return average;
}
void countingMeasurement::start_count_5_data(){
  on_pushButton_clicked();
  measurement_flag = MEASUREMENT_10_AUTO;
}
int countingMeasurement::examine_count_5_data_availability(){
  if(ui->widget_2->global_ispreheat > 0)return 1;
  if(countData5.size() < COUNT_MEASUREMENT_DATA_COUNT){
      if(issample::global_is_sample == WAIT_BE_LOCATION){
          //如果待测样在参考样位置，则使ispreheat的lable2变成“需要采样”
          ispreheat::is_sampling_num = NEED_START_SAMPLING;
          return -1;
        }
      start_count_5_data();
      ispreheat::is_sampling_num = COUNT_MEASUREMENT_DATA_COUNT - countData5.size();
      //qDebug() << ispreheat::is_sampling_num << count_data_5.size();
      return -1;
    }else{
      int count_5_data_stale_count = 0;
      QMap<QDateTime,int>::iterator i;
      //检查5个数据中有几个数据无效
      for(i = countData5.begin();i != countData5.end();i++){
          //数据有效时间为30分钟
          if(i.key().secsTo(QDateTime::currentDateTime()) > 60 *30)
            count_5_data_stale_count++;
        }
      if(count_5_data_stale_count > 0){
          if(issample::global_is_sample == WAIT_BE_LOCATION){
              //如果待测样在参考样位置，则使ispreheat的lable2变成“需要采样”
              ispreheat::is_sampling_num = NEED_START_SAMPLING;
              return -1;
            }
          start_count_5_data();
          ispreheat::is_sampling_num = count_5_data_stale_count;
          return -1;
        }else{
          //只有在正在计数测量并且计数已经得到更新时，才会进入下面的判断条件
          if(flag == 2){// && measurement_flag == MEASUREMENT_10_AUTO
              on_pushButton_2_clicked();//停止测量
            }
          ispreheat::is_sampling_num = 0;
          return 0;
        }
    }
}
int countingMeasurement::clear_count_5_data(){
  countData5.clear();
  //examine_count_5_data_availability();
  return 0;
}
QStringList countingMeasurement::get_count_5_data(){
  QMap<QDateTime,int>::iterator i;
  QStringList list;
  for(i = countData5.begin();i != countData5.end();i++){
      list << QString::number(i.value());
    }
  return list;
}

//自检开始
void countingMeasurement::try_inspectoscope(){
  on_pushButton_clicked();
  try_inspectoscope_flag = 1;
}

//槽函数，由timer控制进行计数测量
void countingMeasurement::count_second(){
#if 1
  if(measurement_flag == MEASUREMENT_SPECTRUM || measurement_flag == MEASUREMENT_CALIBRATE || measurement_flag == MEASUREMENT_SAMPLE){
      stop_measurement_no_transmit();
      return;
    }
#endif
  QString recv_data;
  emit transmit_time(11 * TURN_SECONDS - (turn_counter * TURN_SECONDS - turn_second));

  if(turn_second < 0){
      recv_data = Communciation_Com::receive(5);
      if(recv_data == NULL){
          on_pushButton_2_clicked();
          ErrorCountSave::instance()->addCount(2);
          WinInforListDialog::instance()->showMsg(tr("没有接收到计数数据，已停止测量"));
          QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND * 1000,WinInforListDialog::instance(),SLOT(close()));
          return;
        }else if(recv_data[0] != (char)0x02){
          ErrorCountSave::instance()->addCount(2);
          WinInforListDialog::instance()->showMsg(tr("接收到的计数数据有误，已停止测量") + recv_data);
          on_pushButton_2_clicked();
          return;
        }
      recv_data.remove(0,1);
      show_count_mea->adddata(turn_counter,recv_data);
      recv_data.remove(0,2);
      //只有在是测参考样时才会更新5个计数值
      if(ui->widget->global_is_sample == REFERENCE_BE_LOCATON){
          updata_count_5_average(QDateTime::currentDateTime(),recv_data.toInt());
        }
      if (turn_counter == 11){
          if(1 == try_inspectoscope_flag){
              if(show_count_mea->get_y() <= 0.55 || show_count_mea->get_y() >= 1.45){
                  emit transmit_time(DEFEAT_COUNT);

                  QMessageBox msgbox;
                  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
                  msgbox.setText("计数未通过，需要重新检测么？\n按是重新检测，按取消停止自检");
                  msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                  int ret = msgbox.exec();
                  if(ret == QMessageBox::Yes){
                      emit transmit_time(DEFEAT_COUNT);
                      on_pushButton_2_clicked();
                      usleep(100);
                      on_pushButton_clicked();
                      return;
                    }else if (ret == QMessageBox::Cancel){
                      emit transmit_time(DEFEAT_SPECTOSCOPE);
                      on_pushButton_2_clicked();
                      return;
                    }
                }else{
                  emit transmit_time(SUCCESS_COUNT);
                  try_inspectoscope_flag = 0;
                  on_pushButton_2_clicked();
                  return;
                }
            }
          printer_result();
          show_count_mea->showFullScreen();
          //判断是否是循环计数测量，若是，则在几秒钟之后关闭数据展示窗口，在重新开始另一个测量
          if(measurement_flag == MEASUREMENT_LOOP_COUNT){
              QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND * 1000,show_count_mea,SLOT(close()));
              QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND * 1000,this,SLOT(showFullScreen()));
              QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND  * 1000 + 1000,this,SLOT(on_pushButton_7_clicked()));
            }
          on_pushButton_2_clicked();//停止按钮
          return;
        }

      if(measurement_flag == MEASUREMENT_10_AUTO && turn_counter >= COUNT_MEASUREMENT_DATA_COUNT){
          //当是自动进行的计数测量而且计数达到了COUNT_MEASUREMENT_DATA_COUNT次后停止当前的计数测量
#ifdef DEBUG
          QMessageBox::warning(this,QString(__FILE__),QString(__FUNCTION__) + QString::number(measurement_flag) );
#endif
          on_pushButton_2_clicked();
          emit start_steady_summit_measurement();
          //spectrum_meas->start_steady_summit();
        }
      turn_second = TURN_SECONDS;
      turn_counter += 1;
    }
  QString tmpstr_second = QString("%1 秒").arg(turn_second);
  QString tmpstr_count = QString("第 %1 次").arg(turn_counter);
  ui->label_counter->setText(tmpstr_count);
  ui->label_seconds->setText(tmpstr_second);
  turn_second--;
}

void countingMeasurement::disable_button(bool value){
  ui->pushButton->setDisabled(value);
  ui->pushButton_3->setDisabled(value);
  ui->pushButton_4->setDisabled(value);
  ui->pushButton_6->setDisabled(value);
}

void countingMeasurement::on_pushButton_clicked()
{
  //start button
  if(flag == 1){
      turn_counter = 1;
      turn_second = TURN_SECONDS;
    }
  if (flag == 2) return;
  //judge whether have any other measurement.
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }

  //开始之前停止其他任何测量
  if(on_pushButton_2_clicked() != ALL_RIGHT){
      return;
    }
  //开始计数测量
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  if(Communciation_Com::transmit(ACTIVATINE_COUNT,3) <= 0){
      ErrorCountSave::instance()->addCount(2);
      WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
      return;
    }
  //开始之前清除上一次留下的计数数据
  show_count_mea->clearall();
  ui->label_counter->setText(tr("%1 秒").arg(TURN_SECONDS));
  ui->label_counter->setText("第 1 次");
  flag = 2;
  measurement_flag = MEASUREMENT_COUNT;
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  disable_button(true);
  timer->start(1000);
}

void countingMeasurement::on_pushButton_3_clicked()
{
  this->close();
}

int countingMeasurement::on_pushButton_2_clicked()
{
  //stop button
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  ui->pushButton_7->setText("计数测量");
  ui->pushButton_7->setPalette(QColor(255,255,0,0));
  timer->stop();
  disable_button(false);//改变按钮状态
  measurement_flag = MEASUREMENT_NOTHING;
  try_inspectoscope_flag = 0;
  turn_counter = 1;
  turn_second = TURN_SECONDS;
  ui->label_seconds->setText("");
  ui->label_counter->setText("");
  flag = 1;
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  if(Communciation_Com::transmit(STOP_ORDER,3) <= 0){
      WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
      return ERRNO_COMMUNICATION_1;
    }
  QString recv_data = Communciation_Com::receive(1);
  if(recv_data == NULL){
      ErrorCountSave::instance()->addCount(10);
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + tr("recv Null"));

      return ERRNO_COMMUNICATION_1;
    }
  if(recv_data[1] == (char)0x31){//recv_data[0] == (char)0x98 &&
      ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    }else if(recv_data[1] == (char)0x32){
      ui->widget->change_label_content(WAIT_BE_LOCATION);
    }else if(recv_data[1] == (char)0x33){
      WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT) + tr("recv 0x33"));

      return ERRNO_SILIDING_POSITION;
    }else{
      ErrorCountSave::instance()->addCount(10);
      WinInforListDialog::instance()->showMsg(tr("通信不正常") + ":" + recv_data + "," + QString::number(measurement_flag));
      return ERRNO_COMMUNICATION_1;
    }
  return ALL_RIGHT;
}


void countingMeasurement::stop_measurement_no_transmit(){
  timer->stop();
  disable_button(false);//改变按钮状态
  try_inspectoscope_flag = 0;
  turn_counter = 1;
  turn_second = TURN_SECONDS;
  ui->label_seconds->setText("");
  ui->label_counter->setText("");
  flag = 1;
}


void countingMeasurement::on_pushButton_5_clicked()
{
  //query button
  show_count_mea->showFullScreen();
}

static int move_sliding_count = 0;
static QDateTime last_move_sliding_datetime = QDateTime::currentDateTime();
void countingMeasurement::on_pushButton_6_clicked()
{
  //移进滑板到待测样
  if(move_sliding_count >= 2){
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NOT_ALLOW));
      return;
    }
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  measurement_flag = MEASUREMENT_NOTHING;
  if(Communciation_Com::transmit(IN_SLIDING_PLATE,4) < 0){
      WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
      if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) > 5){
          move_sliding_count = 0;
        }
      return;
    }
  QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
  if(recv_data == NULL){
      ErrorCountSave::instance()->addCount(6);
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT));

    }else if(recv_data[1] == (char)0x32){//recv_data[0] == (char)0x98 &&
      ui->widget->change_label_content(WAIT_BE_LOCATION);
    }else if( recv_data[1] == (char)0x33){//recv_data[0] == (char)0x98 &&
      WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT));
      set_sliding_disabled(false,false);
      emit transmit_move_sliding(false,false);
    }else {
      ErrorCountSave::instance()->addCount(6);
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT)
                                              + recv_data + QString::number(measurement_flag));
    }

  set_sliding_disabled(false,true);
  emit transmit_move_sliding(false,true);
  if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) < 5){
      move_sliding_count++;
    }else{
      move_sliding_count = 0;
    }
  last_move_sliding_datetime = QDateTime::currentDateTime();

}

int countingMeasurement::on_pushButton_4_clicked()
{
  //移出滑板到参考样,
  if(move_sliding_count >= 2){
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NOT_ALLOW));

      if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) > 5){
          move_sliding_count = 0;
        }
      return -2;
    }
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  measurement_flag = MEASUREMENT_NOTHING;
  if(Communciation_Com::transmit(OUT_SLIDING_PLATE,4) < 0){
      WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
      return -1;
    }
  QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
  //qDebug() <<recv_data.toLocal8Bit().data();
  if(recv_data == NULL){
      ErrorCountSave::instance()->addCount(6);
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + tr("\n recv Null"));
      return -1;
    }else if(recv_data[1] == (char)0x31){
      ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    }else if(recv_data[1] == (char)0x33){
      WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT) + tr("\n recv 0x33"));

      set_sliding_disabled(false,false);
      emit transmit_move_sliding(false,false);
      return -1;
    }else {
      ErrorCountSave::instance()->addCount(6);
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT)
                                              + recv_data + "," + QString::number(measurement_flag));
      return -1;
    }
  set_sliding_disabled(true,false);
  emit transmit_move_sliding(true,false);
  if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) < 5){
      move_sliding_count++;
    }else{
      move_sliding_count = 0;
    }
  last_move_sliding_datetime = QDateTime::currentDateTime();
  return ALL_RIGHT;
}
void countingMeasurement::set_sliding_disabled(bool out, bool in){
  ui->pushButton_6->setDisabled(in);
  ui->pushButton_4->setDisabled(out);
}

void countingMeasurement::on_pushButton_7_clicked()
{
  //循环测量按钮
  //if(measurement_flag != MEASUREMENT_NOTHING)return;
  on_pushButton_clicked();//start
  measurement_flag = MEASUREMENT_LOOP_COUNT;
  ui->pushButton_7->setText("循环计数测量");
  ui->pushButton_7->setPalette(QColor(255,0,0,200));
}

void countingMeasurement::printer_result(){
  int enter = 0x0A;
  int just_conut = 0xBFC1E2B2;
  long long count_measurement = 0xBFC1E2B2FDCAC6BCll;//right
  long long reference = 0xf9d1bcbfceb2;
  long long wait_measurement = 0xF9D1A8B6EAB1;

  long long average  = 0xB5D6F9BEBDC6ll;
  long long statistics =0xB5D6C6BCB3CDll;

  printer::printEnd();

  //测量统计值
  printer::transmit(just_conut,4);
  printer::transmit(statistics,6);
  printer::transmit((void *)":  ",3);
  printer::transmit((void *)QString::number(show_count_mea->get_y()).\
                    toLocal8Bit().data(),QString::number(show_count_mea->get_y()).size());
  printer ::transmit(enter,1);

  //测量平均值
  printer::transmit(just_conut,4);
  printer::transmit(average,6);
  printer::transmit((void *)":  ",3);
  printer::transmit((void *)QString::number(show_count_mea->get_average_x()).\
                    toLocal8Bit().data(),QString::number(show_count_mea->get_average_x()).size());
  printer ::transmit(enter,1);

  //日期
  printer::printCurrentDateTime();

  //参考样
  if(ui->widget->global_is_sample ==REFERENCE_BE_LOCATON)printer::transmit(reference,6);
  if(ui->widget->global_is_sample ==WAIT_BE_LOCATION)printer::transmit(wait_measurement,6);
  printer ::transmit(enter,1);

  //计数测量，
  printer::transmit(count_measurement,8);
  printer ::transmit(enter,1);

    //头
  printer::printStart();
}
