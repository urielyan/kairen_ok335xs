#include "samplemeasurement.h"
#include "ui_samplemeasurement.h"
#include "global.h"
#include "datasave.h"

#include "com.h"
#include "showsamplemeasurement.h"
#include "countingmeasurement.h"
#include "input_person_sampleserial.h"

#include <QMessageBox>
#include <countingmeasurement.h>
#include <printer.h>

extern int measurement_flag;

sampleMeasurement::sampleMeasurement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sampleMeasurement)
{
  p_mySettings = MeasurementDataSave::instance();

  ui->setupUi(this);

  count = 3;
  second = 60;
  work_queue = 1;
  showsm = new showsamplemeasurement();

  /*
        flag = 1 ,It's doing measurementing.
        flag = 2 ,It's do nothing.
    */
  flag = 0;
  ui->label_count->setText("");
  ui->label_second->setText("");
  ui->label_rest->hide();

  timer_measurement = new QTimer();
  connect(timer_measurement,SIGNAL(timeout()),this,SLOT(doing_measurement()));

  counting_measurement = new countingMeasurement();
  input_serial = input_person_sampleSerial::instance();


  ui->pushButton_2->setObjectName("stop");
  ui->pushButton->setObjectName("start");
  this->setStyleSheet(""
                      "QPushButton#stop{background-color:rgb(255, 0, 0); color: rgb(255, 255, 255);font-weight:bold;}"
                      "QPushButton#start{background-color:rgb(0, 255, 0); color: rgb(255, 255, 255);font-weight:bold;}");
  ui->label->setObjectName("title");
  ui->label_second->setObjectName("countDown");

  ui->label_5->hide();
  ui->comboBox_standard->hide();
  ui->comboBox_count->setCurrentIndex(1);
  ui->comboBox_time->setCurrentIndex(1);

#ifdef FRIENDLYARM_TINY210
  ui->comboBo_queue->setFont(QFont(FONT_NAME, FONT_SIZE, QFont::Normal));
#endif
#ifdef FORLIN_OK335XS
  ui->comboBo_queue->setFont(QFont(FONT_NAME, 10, QFont::Normal));
  ui->comboBox_time->setFont(QFont(FONT_NAME, 10, QFont::Normal));
#endif


  INIT_LABEL_SIZE_FONT;
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
  ui->label_second->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotStartClicked()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotStopClicked()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotReturnClicked()));
}

sampleMeasurement::~sampleMeasurement()
{
    delete counting_measurement;
    delete timer_measurement;
    delete input_serial;

    delete showsm;
    delete ui;
}

void sampleMeasurement::all_combox_disabled(bool select){
    ui->comboBox_count->setDisabled(select);
    ui->comboBox_time->setDisabled(select);
    ui->comboBo_queue->setDisabled(select);
    ui->comboBox_standard->setDisabled(select);
    ui->pushButton->setDisabled(select);
    ui->pushButton_3->setDisabled(select);
    ui->b_input_serial->setDisabled(select);
}
void sampleMeasurement::doing_measurement(){
    ui->label_count->setText(QString("第 %1 次").arg(change_count));
    ui->label_second->setText(QString("%1 秒").arg(change_second));

    //接收数据，若有误则停止测量
    if(change_second < 0){
        sleep(ui->comboBox_time->currentText().toInt()/30);
        QString recv_data = Communciation_Com::receive(ui->comboBox_time->currentText().toInt()/10 + 3);
        sleep(1);
        if(recv_data == NULL){
            ErrorCountSave::instance()->addCount(3);
            WinInforListDialog::instance()->showMsg(tr("没有接收到含量测量数据，已停止测量！"));
            slotStopClicked();
            return;
        }
        if(recv_data.size() <= 3 || recv_data[0] != (char)0x03){
            ErrorCountSave::instance()->addCount(3);
            WinInforListDialog::instance()->showMsg(tr("含量测量数据有误，已停止测量！"));
            slotStopClicked();
            return;
        }
        recv_data.remove(0,1);

        showsm->add_data(ui->comboBo_queue->currentIndex(),recv_data ,change_count);
        change_count++;
        change_second = second;

        //最后一次测量。
        if(change_count > count){
            ui->label_count->setText("");
            ui->label_second->setText("");
            ui->label_rest->hide();
            timer_measurement->stop();
            measurement_flag = 0;
            sleep(1);
            ui->widget->change_label_content(REFERENCE_BE_LOCATON);
            flag = 2;
            QString tr_data = QString::number(ui->comboBo_queue->currentIndex()) + ";" +\
                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm") + ";" +\
                    ui->comboBox_time->currentText() + ";" +\
                    ui->comboBox_count->currentText() + ";";

            //倒着打印正确的数据
            showsm->show_calculate_storage(tr_data);
            printer_result();

            all_combox_disabled(false);
            return;
        }
        recv_data = Communciation_Com::receive();
    }
    change_second--;
}

void sampleMeasurement::slotReturnClicked()
{
    //return button
    if(1 == flag)return;
    this->close();

}


void sampleMeasurement::slotStartClicked()
{
    //start button
  //如果正在进行含量测量则返回
  if(1 == flag){
      return;
  }

    if (ui->widget_2->global_ispreheat > 0){
        WinInforListDialog::instance()->showMsg(tr("你的仪器需要预热，请查看下方"));
        return;
    }

    if(ui->widget_2->is_sampling_num > 0){
        WinInforListDialog::instance()->showMsg(tr("含量测量需要参考样的实时计数，但是此时参考样计数已经失效,所以请等待参考样采样完毕"));
        return;
    }

    if(measurement_flag == MEASUREMENT_10_AUTO){
        //正在进行10分钟的自动计数测量，发送信号
        emit transmit_stop_auto_count();
    }
    //如果计数的5个数据没有得到更新，则返回。
    if(counting_measurement->examine_count_5_data_availability() < 0){
        WinInforListDialog::instance()->showMsg(tr("样品已过期，请注意下方，等待采样完成"));
        return;
    }

    QString workCurve;
    //选择的kb值没有数据则不发送测量信号
     if(ui->comboBo_queue->currentIndex() != 0){
        workCurve = p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(
                                            ui->comboBo_queue->currentText().toInt())).toString();
        if((workCurve.split(";")[0] == NULL) || (workCurve.split(";")[1] == NULL)){
            WinInforListDialog::instance()->showMsg(tr("您选择的工作曲线有误，请重新选择"));
            return;
        }
     }else{
         //当选择自动选择工作曲线时，判断123三条工作曲线是否有值。若没有则不进行含量测量
        workCurve = p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(1)).toString();
        if((workCurve.split(";")[0] == NULL) || (workCurve.split(";")[1] == NULL)){
            WinInforListDialog::instance()->showMsg(tr("您不能自动选择“自动选择工作曲线”，工作曲线1没有kb值，请重新选择"));
            return;
        }
        workCurve = p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(2)).toString();

        if((workCurve.split(";")[0] == NULL) || (workCurve.split(";")[1] == NULL)){
            WinInforListDialog::instance()->showMsg(tr("您不能自动选择“自动选择工作曲线”，工作曲线2没有kb值，请重新选择"));
            return;
        }
        workCurve = p_mySettings->value(MYSETTINGS_CALIBRATE_RESULT_REAL_KBR(6)).toString();
        if((workCurve.split(";")[0] == NULL) || (workCurve.split(";")[1] == NULL)){
            WinInforListDialog::instance()->showMsg(tr("您不能自动选择“自动选择工作曲线”，工作曲线6没有kb值，请重新选择"));
            return;
        }
     }
    //开始之前停止任何测量,并刷新输入输出缓冲区
     if(slotStopClicked() != ALL_RIGHT)return;

    //发送测量信号
    measurement_flag = MEASUREMENT_SAMPLE;
    tcflush(Communciation_Com::fd,TCIOFLUSH);
    long long tmp = 0x03FELL | ((ui->comboBox_time->currentIndex() + 0x31) << 16);
    tmp |= ((ui->comboBox_count->currentIndex() + 0x31) << 24);
    tmp |= 0xffLL << 32;
    if (Communciation_Com::transmit(tmp,5) <= 0){
        WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
        measurement_flag = MEASUREMENT_NOTHING;
        return;
    }
    //如果没有收到滑板到位指令，则返回
    QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
    if(recv_data == NULL){
        WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + tr("recv Null"));
        measurement_flag = MEASUREMENT_NOTHING;
        return;
    }
    if(recv_data[1] == (char)0x33){
        measurement_flag = MEASUREMENT_NOTHING;
//        ui->widget->global_is_sample = MACHINE_MALFUNCTION;
        all_combox_disabled(false);
        WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT) + tr("recv 0x33"));
        slotStopClicked();
        return;
    }else if(recv_data[1] == (char)0x32){//recv_data[0] == (char)0x98 &&
        all_combox_disabled(true);
        //若上述条件都成立，则开始含量测量
        ui->widget->change_label_content(WAIT_BE_LOCATION);
        count = ui->comboBox_count->currentText().toInt();
        second = change_second= ui->comboBox_time->currentText().toInt();
        ui->label_rest->show();
        work_queue = ui->comboBo_queue->currentText().toInt();
        change_count = 1;

        flag = 1;
        timer_measurement->start(1000);
        measurement_flag = MEASUREMENT_SAMPLE;
        tcflush(Communciation_Com::fd,TCIOFLUSH);
        showsm->clear_tablewidget();
        return;
    }else{
        ErrorCountSave::instance()->addCount(3);
        WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + recv_data);

        measurement_flag = MEASUREMENT_NOTHING;
        all_combox_disabled(false);
        slotStopClicked();
        return;
      }
}

int sampleMeasurement::slotStopClicked()
{
    //stop button
      if(measurement_flag != MEASUREMENT_NOTHING){
          emit transmit_stop_auto_count();
      }
    ui->label_count->setText("");
    ui->label_second->setText("");
    ui->label_rest->hide();
    all_combox_disabled(false);
    flag = 2;
    timer_measurement->stop();
    measurement_flag = MEASUREMENT_NOTHING;
    ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    tcflush(Communciation_Com::fd,TCIOFLUSH);
    if(Communciation_Com::transmit(STOP_ORDER,3) <= 0){
        WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
        return ERRNO_COMMUNICATION_1;
      }
    QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
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

void sampleMeasurement::on_pushButton_4_clicked()
{
    //query button
    showsm->showFullScreen();
}

void sampleMeasurement::on_b_input_serial_clicked()
{
    input_serial->showFullScreen();
}

void sampleMeasurement::printer_result(){

  int enter = 0x0A;
  int second_tr = 0xEBC3;
  int repeat_count = 0xCEB4;

  long long samplemeasurement = 0xBFC1E2B2BFC1ACBAll;//right

  long long work_line1 = 0xDFCFFAC7F7D7A4B9LL;
  int work_line2 = 0xC5BA;
  long long measurement_time = 0xE4BCB1CABFC1E2B2ll;
  long long repeat_time = 0xFDCACEB4B4B8D8D6ll;

  //重复次数
  printer::transmit(repeat_time,8);
  printer::transmit((void *)":    ",5);
  printer::transmit((void *)(ui->comboBox_count->currentText().toLocal8Bit().data()),ui->comboBox_count->currentText().size());
  printer::transmit((void *)"    ",4);
  printer::transmit(repeat_count,2);
  printer ::transmit(enter,1);

  //测量时间
  printer::transmit(measurement_time,8);
  printer::transmit((void *)":    ",5);
  printer::transmit((void *)(ui->comboBox_time->currentText().toLocal8Bit().data()),ui->comboBox_time->currentText().size());
  printer::transmit((void *)"   ",3);
  printer::transmit(second_tr,2);
  printer ::transmit(enter,1);

  //工作曲线
  printer::transmit(work_line1,8);
  printer::transmit(work_line2,2);
  printer::transmit((void *)":  ",4);
  printer::transmit((char)(showsm->get_real_curve() + 0x30));
  printer ::transmit(enter,1);

  //日期
  printer::printCurrentDateTime();

  printer::transmit(samplemeasurement,8);
  printer ::transmit(enter,1);

  printer::instance()->printStart();
}
