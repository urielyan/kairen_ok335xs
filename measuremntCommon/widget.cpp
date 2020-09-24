#include "common/sliding.h"

#include <QDate>
#include<QTime>
#include<QTimer>
#include<QDebug>
#include <QMessageBox>

#include "datasave.h"
#include "widget.h"
#include "ui_widget.h"
#include "global.h"
#include "com.h"
#include "wininforlistdialog.h"
#include "countingmeasurement.h"
#include "spectrummeasurement.h"
#include "dataquery.h"
#include "samplemeasurement.h"
#include "passwd.h"
#include "inspectoscope.h"

/*区分现在是在做什么:
 * 每次测量之后都要把measurement_flag清零
 *  measurement_flag = 0 :不做任何事
 *  measurement_flag = 1 :能谱测量
 *  measurement_flag = 2 :计数测量
 *  measurement_flag = 3 :标定测量
 *  measurement_flag = 4 :含量测量
 *  measurement_flag = 5 :10分钟自动进行的计数参考样测量
 *  measurement_flag = 6 :循环计数测量
*/
int measurement_flag = 0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    p_mySettings = MeasurementDataSave::instance();
    Communciation_Com *tmp_com = new Communciation_Com(this);
    tmp_com->transmit(0, 0);
    ui->setupUi(this);
    cm = new countingMeasurement();
    cm_auto = new countingMeasurement();
    sm = new spectrummeasurement();
    dq = new dataquery();
    samplem = new sampleMeasurement();
    pw = new passwd();
    inspectoscope_measurement = new inspectoscope();
    //ui->b4->hide();
    //ui->b1->setDisabled(true);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_count_5_data()));
    timer->start(1000 * 2);
    QTimer *timer_auto_count_measurement = new QTimer(this);
    connect(timer_auto_count_measurement, SIGNAL(timeout()), this, SLOT(auto_start_count_measument()));
    timer_auto_count_measurement->start(1000);
    //当按其他测量时自动的计数测量会停止。通过下面两个连接一级一级的吧信号传到cm_auto对象中。
    connect(samplem, SIGNAL(transmit_stop_auto_count()), cm_auto, SLOT(stop_measurement_no_transmit()));
    connect(pw, SIGNAL(transmit_stop_auto_count()), cm_auto, SLOT(stop_measurement_no_transmit()));
    connect(pw, SIGNAL(transmit_stop_auto_count()), cm, SLOT(stop_measurement_no_transmit()));
    connect(this, SIGNAL(transmit_stop_auto_count()), cm, SLOT(stop_measurement_no_transmit()));
    connect(this, SIGNAL(transmit_stop_auto_count()), cm_auto, SLOT(stop_measurement_no_transmit()));
    //当按其他测量时自动的稳峰测量会停止。通过下面两个连接一级一级的吧信号传到cm_auto对象中。
    connect(this, SIGNAL(transmit_stop_auto_count()), sm, SLOT(stop_measurement_no_transmit()));
    connect(pw, SIGNAL(transmit_stop_auto_count()), sm, SLOT(stop_measurement_no_transmit()));
    connect(samplem, SIGNAL(transmit_stop_auto_count()), sm, SLOT(stop_measurement_no_transmit()));
    get_slide_current_position();
    //开始一个含量测量和移动滑板会使稳峰测量停止
    connect(samplem, SIGNAL(transmit_stop_auto_count()), pw, SIGNAL(transmit_stop_auto_count()));
    connect(this, SIGNAL(transmit_stop_auto_count()), pw, SIGNAL(transmit_stop_auto_count()));
    //在计数和能谱测量下按移动滑板会使主菜单下的移动滑板按钮改变状态
    connect(pw, SIGNAL(change_widget_sliding(bool, bool)), this, SLOT(set_sliding_disabled(bool, bool)));
    //在主菜单下按移动滑板会使计数和能谱测量下的移动滑板按钮改变状态
    connect(this, SIGNAL(change_sliding_disabled(bool, bool)), pw, SIGNAL(change_sliding_disabled(bool, bool)));
    //当一个自动计数测量结束时开始一个测量峰位的能谱测量，下面是连接两个类中的信号和槽的
    connect(cm_auto, SIGNAL(start_steady_summit_measurement()), sm, SLOT(start_steady_summit()));
    connect(sm, SIGNAL(transmit_steady_summit_result(double)),
            this, SLOT(judge_spectrument_measurement_result(double)));

    INIT_LABEL_SIZE_FONT;
    ui->label->setFont(QFont("symbol.ttf", FONT_SIZE * 2, QFont::Normal));
    ui->label->setObjectName("title");
    Sliding::insertSliderButtonPairs(ui->b4, ui->b6);

    {
    QString line_data = QString::number(p_mySettings->value(MYSETTINGS_COUNT_VOLTAGE).toInt());

    long long transmit_data = 0xff;
    for(int i = line_data.size() - 1;i >= 0 ;i--){
        //把需要给下位机的数据或到transmit上
        transmit_data = (transmit_data << 8);
        transmit_data = transmit_data | (long long)line_data[i].toLatin1();
        qDebug()<<line_data.size() << line_data[i];
    }
    for(int i = 0;i < 4 - line_data.size();i++){
        //补齐
        transmit_data = (transmit_data << 8) | 0x30;
    }
    transmit_data = (transmit_data << 8) | SETUP_COUNT_VOLTAGE;
    transmit_data = (transmit_data << 8) | 0xfe;



    tcflush(Communciation_Com::fd,TCIOFLUSH);
    if(Communciation_Com::transmit(transmit_data,7) <= 0){
        WinInforListDialog::instance()->showMsg(tr("初始化计数管高压,命令发送失败。"));
        return;
      }
    }
}

Widget::~Widget()
{
    delete inspectoscope_measurement;
    delete pw;
    delete samplem;
    delete dq;
    delete sm;
    delete cm_auto;
    delete cm;
    delete ui;
}

/*计数测量的自动模块：
 *  当10 seconds没有任何测量，则自动进行计数测量，不显示计数测量页面
 *  通过measuremect_flag 这个全局变量判断是否有操作
 */
void Widget::auto_start_count_measument()
{
    if(ui->widget_ispreheat->global_ispreheat > 0) {
        //
        return;
    }
    if(ui->widget_ispreheat->is_sampling_num > 0) {
        return;
    }
    static int no_behaviour_count_second;
    if(measurement_flag == MEASUREMENT_NOTHING) {
        no_behaviour_count_second++;
    } else {
        no_behaviour_count_second = 0;
    }
    if(no_behaviour_count_second > NO_BEHAVIOUR_START_COUNT) {
        //cm_auto->showFullScreen();
        no_behaviour_count_second = 0;
        //若待测样在测量位置，则不测量
        if(ui->widget_issample->global_is_sample == WAIT_BE_LOCATION) {
            no_behaviour_count_second = NO_BEHAVIOUR_START_COUNT / 2;
            return;
        }
        //开始测量
        if(measurement_flag != MEASUREMENT_NOTHING)return;
        cm_auto->on_pushButton_clicked();
        measurement_flag = MEASUREMENT_10_AUTO;
    }
}

void Widget::update_count_5_data()
{
    if(measurement_flag != MEASUREMENT_10_AUTO &&  measurement_flag != MEASUREMENT_NOTHING) {
        ispreheat::is_sampling_num = 0;
        return;
    }
    cm->examine_count_5_data_availability();
}
void Widget::on_b1_clicked()
{
    //sample measurement.
    samplem->showFullScreen();
}

void Widget::on_b4_clicked()
{
    //移进滑板到待测样
    if(measurement_flag != MEASUREMENT_NOTHING) {
        emit transmit_stop_auto_count();
        measurement_flag = MEASUREMENT_NOTHING;
    }
    Sliding::moveSlider(Sliding::In);
}

int Widget::on_b6_clicked()
{
    //移出滑板到参考样,
    if(measurement_flag != MEASUREMENT_NOTHING) {
        emit transmit_stop_auto_count();
        measurement_flag = MEASUREMENT_NOTHING;
    }
    if(Sliding::moveSlider(Sliding::Out) != Sliding::SlidingSuccess) {
        return false;
    }
    return true;
}

void Widget::set_sliding_disabled(bool out, bool in)
{
    ui->b6->setDisabled(out);
    ui->b4->setDisabled(in);
}

void Widget::on_b5_clicked()
{
    //自检功能
#if  1
    QMessageBox msgbox;
    msgbox.setFont(QFont(FONT_NAME, FONT_SIZE, QFont::Normal));
    //QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND * 1000,&msgbox,SLOT(close()));
    msgbox.setText("您需要等待大约5分钟以便让机器自检完成,其中会进行一个能谱测量和一个计数测量。若发现仪器故障会自动进行修复。");
    msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    int ret = msgbox.exec();
#endif
    if(ret == QMessageBox::Ok) {
        if (on_b6_clicked() == false) {
            return;
        }
        //if(measurement_flag == 4)return;
        connect(sm, SIGNAL(transmit(int)), inspectoscope_measurement, SLOT(receive_time(int))); //每测一个能谱计数则发送给自检画面并显示
        connect(sm, SIGNAL(transmit_count_time(int)), inspectoscope_measurement, SLOT(receive_count_time(int))); //每测一个计数计数则发送给自检画面并显示
        //当按自检画面的结束键时停止计数和能谱测量
        connect(inspectoscope_measurement, SIGNAL(stop_all_inspectoscope()), sm, SLOT(stop_inspectoscope()));
        connect(sm, SIGNAL(transmit_stop_auto_count()), cm_auto, SLOT(stop_measurement_no_transmit()));
        connect(sm, SIGNAL(transmit_stop_auto_count()), cm, SLOT(stop_measurement_no_transmit()));
        inspectoscope_measurement->showFullScreen();
        sm->try_inspectoscope();
    }
}

void Widget::on_b2_clicked()
{
    dq->showFullScreen();
}

void Widget::on_b3_clicked()
{
    pw->showpass();
}

void Widget::get_slide_current_position()
{
    on_b6_clicked();
}

static int steady_summit_count = 0;
void Widget::judge_spectrument_measurement_result(double summit)
{
    //弹出那个更换计数管高压的原因是，我会在没有任何测量的情况下 后台进行稳压测量， 稳压测量会判断峰值是否在1.4-1.6之间，若不在会将当前合适的 计数管高压值发给下位机，若连续三次计数峰值不在1.4-1.6 可能计数管坏了

    if(summit > 1.6 || summit < 1.4) {
        int calibrate_summit = (1.5 - summit) * 40 + p_mySettings->value(MYSETTINGS_COUNT_VOLTAGE).toInt();
        if(calibrate_summit > 700 && summit < 1.4) {
            qDebug() << "需要更换计数管高压, summit = "<< summit;
            //WinInforListDialog::instance()->showMsg(tr("需要更换计数管高压,请联系厂家18855953618"));
            return;
        }
        steady_summit_count += 1;
        if(steady_summit_count >= 3) {
            if(pw->set_count_voltage(calibrate_summit) == ALL_RIGHT) {
                p_mySettings->setValue(MYSETTINGS_COUNT_VOLTAGE, calibrate_summit);
                return;
            }
            return;
        }
        sm->start_steady_summit();
    } else {
        steady_summit_count = 0;
    }
}

#include "countingmeasurement.h"
#include "spectrummeasurement.h"
void Widget::on_countMeasurementButton_clicked()
{
    QEventLoop loop;
    static countingMeasurement w;
    w.showFullScreen();

    cm_auto->stop_measurement_no_transmit();
    sm->stop_measurement_no_transmit();

    connect(&w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();
    loop.exit();
}

void Widget::on_spectrumMeasurementButton_clicked()
{
    QEventLoop loop;
    static spectrummeasurement w;
    cm_auto->stop_measurement_no_transmit();
    sm->stop_measurement_no_transmit();

    connect(&w, SIGNAL(closed()), &loop, SLOT(quit()));
    w.showFullScreen();
    loop.exec();
    loop.exit();
}
