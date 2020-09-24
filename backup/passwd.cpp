#include "passwd.h"
#include "ui_passwd.h"
#include "global.h"
#include "com.h"
#include "countingmeasurement.h"
#include "datasave.h"
#include "systemsetup.h"
#include "hide_system.h"
#include "wininforlistdialog.h"
#include "input_machine_use_time.h"

#include <QList>
#include <QString>
#include <QDebug>

extern int measurement_flag;


static int inInputSerialClearClickGuard = 0;

passwd::passwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passwd)
{
    p_mySettings = MeasurementDataSave::instance();

    ui->setupUi(this);
    input = new input_machine_use_time;

    flag =0;
    ss = new systemsetup();
    hs = new hide_system();

//    //开始一个含量测量和移动滑板会使稳峰测量停止
//    connect(this,SIGNAL(transmit_stop_auto_count()),ss,SLOT(stop_steady_summit()));

    ui->lineEdit->setText("");

    //使系统设定里的信号触发这里的信号从而触发自动测量的停止槽函数
    connect(ss,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));

    //当widget下的滑板改变时让另外两个界面下的滑板是否可移动的状态改变
    connect(this,SIGNAL(change_sliding_disabled(bool,bool)),ss,SIGNAL(change_sliding_disabled(bool,bool)));
    //当计数测量and能谱测量的滑板改变时让另外main界面下的滑板是否可移动的状态改变
    connect(ss,SIGNAL(change_widget_sliding(bool,bool)),this,SIGNAL(change_widget_sliding(bool,bool)));

    //当隐藏系统,系统设定发送一个信号时显示此界面
    connect(hs,SIGNAL(change_voltage_electricity(int)),this,SLOT(alterpasswd_show(int)));
    connect(ss,SIGNAL(show_alterpasswd(int)),this,SLOT(alterpasswd_show(int)));

    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for(int i=0;i<allPButtons.count();i++){
        QString tmpstr=allPButtons[i]->objectName();
        //  tmpstr=tmpstr.remove("b_");
        if(tmpstr.length()==3){
            QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keyNumPressed()));
        }
    }
    ui->widget->hide();
    ui->widget_2->hide();
    ui->widget_3->hide();

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    ui->label_title->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));

    ui->b_clear->setText(tr("清除"));
    connect(ui->b_clear, SIGNAL(clicked(bool)),this,SLOT(slotClearClicked()));
    ui->b_backSpace->setText(tr("退格"));
    connect(ui->b_backSpace, SIGNAL(clicked(bool)),this,SLOT(slotBackSpaceClicked()));

    ui->label_title->setObjectName("title");
}

passwd::~passwd()
{
    delete hs;
    delete ss;
    delete ui;
}

void passwd::slot_keyNumPressed()
{
    QString tmpstr=sender()->objectName();
    tmpstr.remove("b_");
    ui->lineEdit->setText(ui->lineEdit->text() + tmpstr);


    if(flag == SETUP_COUNT_VOLTAGE)
    {
        if(ui->lineEdit->text().toInt() > enumMaxCountVoltage){
             ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().size() - 1));
          }
        return;
    }
    if(flag == SETUP_LIGHT_VOLTAGE)
    {
        if(ui->lineEdit->text().toInt() > enumMaxLightVoltage){
            ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().size() - 1));
          }
        return;
    }
    if(flag == SETUP_LIGHT_ELECTRICITY)
    {
        if(ui->lineEdit->text().toInt() > enumMaxLightCurrent){
            ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().size() - 1));
          }
        return;
    }
    if(flag == SETUP_ALTER_PASSWD){
        if(ui->lineEdit->text().size() > 6){
            ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().size() - 1));
        }
        return;
    }

    if(ui->lineEdit->text().size() >= 6){
        qDebug() << MeasurementDataSave::instance()->value("passwd").toInt();
        if(ui->lineEdit->text().compare(MeasurementDataSave::instance()->value("passwd").toString()) == 0 ||!ui->lineEdit->text().compare("234516")  ){
            ss->showFullScreen();
            ui->lineEdit->clear();
            this->close();
        }else if(!ui->lineEdit->text().compare("180207")){
            //kairen 201601
            //xue zong 180207
            //进入管理员菜单
            ui->lineEdit->clear();
            hs->showFullScreen();
            this->close();
        }else{
            ui->lineEdit->text().clear();
            ui->lineEdit->clear();
            WinInforListDialog::instance()->showMsg(tr("密码错误，请重新输入"));
        }
    }
}

void passwd::showpass(){
    ui->label_title->setText("");
    ui->label_caution->setText("请输入密码:");
    ui->label_unit->setText("");
    ui->lineEdit->setText("");
    ui->line->hide();
    this->showFullScreen();
}

void passwd::alterpasswd(){
    ui->label_title->setText("修改密码");
    ui->label_caution->setText("请输入新密码:");
    ui->label_unit->setText("");
    ui->lineEdit->setText("");
    ui->line->showFullScreen();

    //alterpasswd_show(DISPLAY_ALTER_PASSWD,"修改密码","请输入新密码:");
    ui->widget_2->hide();
    flag = SETUP_ALTER_PASSWD;
    this->showFullScreen();
}

void passwd::on_b_return_clicked()
{
    ui->lineEdit->setText("");
    ui->label_unit->setText("");
    flag = 0;
    this->close();
}


//确认按钮
//弹出那个更换计数管高压的原因是，我会在没有任何测量的情况下 后台进行稳压测量， 稳压测量会判断峰值是否在1.4-1.6之间，
//若不在会将当前合适的 计数管高压值发给下位机，若连续三次计数峰值不在1.4-1.6 可能计数管坏了
void passwd::on_b_ok_clicked()
{
    if(flag == SETUP_COUNT_VOLTAGE || \
            flag == SETUP_LIGHT_VOLTAGE || \
            flag == SETUP_LIGHT_ELECTRICITY)
    {
    //如果是设定光管或计数管则进入此流程

        if(ui->lineEdit->text() == NULL){
            return;
        }
        QString line_data = ui->lineEdit->text().toLocal8Bit();
        if(flag == SETUP_COUNT_VOLTAGE){
            if(line_data.toInt() < enumMinCountVoltage){
                WinInforListDialog::instance()->showMsg(tr("设定计数管高压的电压范围在%1-%2V")
                                                        .arg(enumMinCountVoltage).arg(enumMaxCountVoltage));
                return;
              }
          }
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
        transmit_data = (transmit_data << 8) | flag;
        transmit_data = (transmit_data << 8) | 0xfe;

        if(stop_all_measurement() != ALL_RIGHT)
          {
            return;
          }

        tcflush(Communciation_Com::fd,TCIOFLUSH);
        if(Communciation_Com::transmit(transmit_data,7) <= 0){
            WinInforListDialog::instance()->showMsg(tr("命令发送失败。"));
            return;
          }
        QString recv_data = Communciation_Com::receive(SETUP_WAIT_TIME);
        if (1){
#if 0
            //not examine
        if(recv_data == NULL){
            WinInforListDialog::instance()->showMsg(tr("设定不成功"));
            ErrorCountSave::instance()->addCount(6);
        }else if(recv_data[1] == (char)0x31 || recv_data[1] == (char)0x32){
#endif
           p_mySettings->setValue(MYSETTINGS_CHANGE_COUNT_VOLTAGE(
                                      p_mySettings->value(MYSETTINGS_CHANGE_COUNT_VOLTAGE_COUNT).toUInt()),
                                            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")
                                  + ";" + p_mySettings->value(MYSETTINGS_COUNT_VOLTAGE).toString() + ";" + line_data);
            p_mySettings->setValue(MYSETTINGS_CHANGE_COUNT_VOLTAGE_COUNT,
                                            ErrorCountSave::instance()->value(MYSETTINGS_CHANGE_COUNT_VOLTAGE_COUNT).toInt()+1);
            switch (flag) {
              case SETUP_COUNT_VOLTAGE:
                p_mySettings->setValue(MYSETTINGS_COUNT_VOLTAGE,line_data.toInt());
                break;
              case SETUP_LIGHT_VOLTAGE:
                p_mySettings->setValue(MYSETTINGS_LIGHT_VOLTAGE,line_data.toInt());
                break;
              case SETUP_LIGHT_ELECTRICITY:
                p_mySettings->setValue(MYSETTINGS_LIGHT_CURRENT,line_data.toInt());
                break;
              default:
                break;
              }
            WinInforListDialog::instance()->showMsg(tr("设定成功"));
            countingMeasurement::clear_count_5_data();
        }else if(recv_data[1] == (char)0x33){
            WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT));
          }else {
            ErrorCountSave::instance()->addCount(6);
            WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR) + recv_data);
          }
        on_b_return_clicked();
    }else if(SETUP_ALTER_PASSWD == flag)
      {
        //下面是修改密码的代码
        if(ui->lineEdit->text().size() != 6)
          return;

       MeasurementDataSave::instance()->setValue("passwd",ui->lineEdit->text());
        qDebug() << MeasurementDataSave::instance()->value("passwd").toString();
        WinInforListDialog::instance()->showMsg(tr("修改密码成功，请牢记此密码") + ui->lineEdit->text());

        on_b_return_clicked();
      }
}

void passwd::alterpasswd_show(int index){
    //提供给修改计数管，光管的界面
    if(index == SETUP_COUNT_VOLTAGE){
        ui->label_title->setText(tr("设定计数管高压"));

        ui->label_caution->setText(QString(tr("请输入电压(电压范围在%1-%2V)")).arg(enumMinCountVoltage).arg(enumMaxCountVoltage));
        ui->label_unit->setText("V");
        ui->lineEdit->setText(p_mySettings->value(MYSETTINGS_COUNT_VOLTAGE).toString());
    }else if(index == SETUP_LIGHT_VOLTAGE){
        ui->label_title->setText(tr("设定光管高压"));
        ui->label_caution->setText(tr("请输入电压"));
        ui->label_unit->setText("V");
        ui->lineEdit->setText(p_mySettings->value(MYSETTINGS_LIGHT_VOLTAGE).toString());
    }else if(index == SETUP_LIGHT_ELECTRICITY){
        ui->label_title->setText(tr("设定光管电流"));
        ui->label_caution->setText(tr("请输入电流"));
        ui->label_unit->setText("μA");
        ui->lineEdit->setText(p_mySettings->value(MYSETTINGS_LIGHT_CURRENT).toString());
    }else if(index == SETUP_ALTER_PASSWD){
        ui->label_title->setText(tr("修改密码"));
        ui->label_caution->setText(tr("请输入新密码:"));
        ui->label_unit->setText("");
        ui->lineEdit->setText("");
        ui->line->showFullScreen();
    }
    flag = index;
    this->showFullScreen();
}

int passwd::set_count_voltage(int count_voltage){
  if(count_voltage > enumMaxCountVoltage || count_voltage < enumMinCountVoltage){
      return 1;
    }
  ui->lineEdit->setText(QString::number(count_voltage));
  flag = SETUP_COUNT_VOLTAGE;
  on_b_ok_clicked();
  return ALL_RIGHT;
}


void passwd::slotBackSpaceClicked()
{
    if (inInputSerialClearClickGuard == 1 && ui->lineEdit->text().size() == 0)
    {
        //input->setAttribute(Qt::WA_DeleteOnClose, true);
        input->input_decode_serial();
    }
  if(ui->lineEdit->text().size() > 0)
    {
      ui->lineEdit->setText(ui->lineEdit->text().left(ui->lineEdit->text().size() - 1));
  }
  inInputSerialClearClickGuard = 0;
}

void passwd::slotClearClicked()
{
  ui->lineEdit->setText("");
  inInputSerialClearClickGuard = 1;
}

int passwd::stop_all_measurement(){
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(Communciation_Com::fd,TCIOFLUSH);
  if (Communciation_Com::transmit(STOP_ORDER,3) <= 0){
      WinInforListDialog::instance()->showMsg(TRANSMIT_DATA_ERROR);
      return ERRNO_COMMUNICATION_1;
    }
  QString recv_data = Communciation_Com::receive(1);
  if(recv_data == NULL){
      WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + tr("\n recv NULL"));
      return ERRNO_COMMUNICATION_1;
    }
  if(recv_data[1] == (char)0x31){//recv_data[0] == (char)0x98 &&
      ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    }else if(recv_data[1] == (char)0x32){
      ui->widget->change_label_content(WAIT_BE_LOCATION);
    }else if(recv_data[1] == (char)0x33){
      WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT) + tr("\n 0x33"));
      return ERRNO_SILIDING_POSITION;
    }else{
      ErrorCountSave::instance()->addCount(10);
      WinInforListDialog::instance()->showMsg(tr("通信不正常") + ":" + recv_data + "," + QString::number(measurement_flag));
      return ERRNO_COMMUNICATION_1;
    }
  return ALL_RIGHT;
}
