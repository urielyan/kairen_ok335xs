#include "passwd.h"
#include "ui_passwd.h"
#include "global.h"
#include "com.h"
#include "countingmeasurement.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

extern int measurement_flag;

static QString tmppasswd("");
static QString tmplineedit("");

passwd::passwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passwd)
{
    ui->setupUi(this);
    flag =0;
    ss = new systemsetup();
    hs = new hide_system();

//    //开始一个含量测量和移动滑板会使稳峰测量停止
//    connect(this,SIGNAL(transmit_stop_auto_count()),ss,SLOT(stop_steady_summit()));
    //mysettings.clear();
    if (!mysettings.contains("passwd")){
        mysettings.setValue("passwd",111111);
    }
    ui->lineEdit->setText(tmplineedit);

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
}

passwd::~passwd()
{
    delete hs;
    delete ss;
    delete ui;
}

void passwd::slot_keyNumPressed(){
    QString tmpstr=sender()->objectName();
    tmpstr.remove("b_");
    tmppasswd += tmpstr;

    if(flag == SETUP_COUNT_VOLTAGE)
    {
        if(tmppasswd.toInt() > 700){
             tmppasswd.chop(1);
          }
        ui->lineEdit->setText(tmppasswd);
        return;
    }
    if(flag == SETUP_LIGHT_VOLTAGE)
    {
        if(tmppasswd.toInt() > 15000){
             tmppasswd.chop(1);
          }
        ui->lineEdit->setText(tmppasswd);
        return;
    }
    if(flag == SETUP_LIGHT_ELECTRICITY)
    {
        if(tmppasswd.toInt() > 1000){
             tmppasswd.chop(1);
          }
        ui->lineEdit->setText(tmppasswd);
        return;
    }
    if(flag == DISPLAY_ALTER_PASSWD){
        if(tmppasswd.size() > 6){
            tmppasswd.chop(1);
        }
        ui->lineEdit->setText(tmppasswd);
        return;
    }

    tmplineedit += "*";
    ui->lineEdit->setText(tmplineedit);
    if(tmppasswd.size() >= 6){
        //qDebug() << mysettings.value("passwd").toInt();
        if(tmppasswd.compare(mysettings.value("passwd").toString()) == 0){
            ss->showFullScreen();
            tmppasswd.clear();
            tmplineedit.clear();
            ui->lineEdit->clear();
            this->close();
        }else if(!tmppasswd.compare("666666")){
            //进入管理员菜单
            tmppasswd.clear();
            tmplineedit.clear();
            ui->lineEdit->clear();
            hs->showFullScreen();
            this->close();
        }else{
            tmppasswd.clear();
            tmplineedit.clear();
            ui->lineEdit->clear();
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            QTimer::singleShot(MESSAGEBOX_AUTO_CLOSE_SECOND * 1000,&msgbox,SLOT(close()));
            //msgbox.removeButton(QMessageBox::Ok);
            //msgbox.setStandardButtons(QMessageBox::NoButton);
            msgbox.setText("密码错误，请重新输入");
            //this->close();
            msgbox.exec();
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
    flag = DISPLAY_ALTER_PASSWD;
    this->showFullScreen();
}

void passwd::on_b_return_clicked()
{
    tmppasswd = "";
    tmplineedit = "";
    ui->lineEdit->setText("");
    ui->label_unit->setText("");
    flag = 0;
    this->close();
}


//确认按钮
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
            if(line_data.toInt() < COUNT_VOLTAGE_MIN){
                QMessageBox msgbox;
                msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
                msgbox.setText(QString("设定计数管高压的电压范围在%1-%2V").arg(COUNT_VOLTAGE_MIN).arg(COUNT_VOLTAGE_MAX));
                msgbox.exec();
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

        stop_all_measurement();

        tcflush(com::fd,TCIOFLUSH);
        if(com::transmit(transmit_data,7) <= 0){
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText("transmit err!");
            msgbox.exec();
            return;
          }
        QString recv_data = com::receive(SETUP_WAIT_TIME);
        if(recv_data == NULL){
            QSettings communication_err_data("shanghaikairen","communication_error");
            communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText("设定不成功");
            msgbox.exec();
        }else if(recv_data[1] == (char)0x31){
            QSettings communication_err_data("shanghaikairen","communication_error");
            communication_err_data.setValue(QString("change_count_voltage_")+
                                            communication_err_data.value("change_count_voltage_count").toString(),
                                            QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")+ ";" + mysettings.value("count_voltage").toString()
                                            + ";" + line_data);
            communication_err_data.setValue("change_count_voltage_count",
                                            communication_err_data.value("change_count_voltage_count").toInt()+1);
            mysettings.setValue("count_voltage",line_data.toInt());
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText("设定成功");
            msgbox.exec();
            countingMeasurement::clear_count_5_data();
        }else if(recv_data[1] == (char)0x32){
            QSettings communication_err_data("shanghaikairen","communication_error");
            communication_err_data.setValue(QString("change_count_voltage_")+
                                            communication_err_data.value("change_count_voltage_count").toString(),
                                            QDateTime::currentDateTime().toString()+ ";" + communication_err_data.value("count_voltage").toString()
                                            + ";" + line_data);
            communication_err_data.setValue("change_count_voltage_count",
                                            communication_err_data.value("change_count_voltage_count").toInt()+1);
            mysettings.setValue("count_voltage",line_data.toInt());
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText("设定成功");
            msgbox.exec();
            countingMeasurement::clear_count_5_data();
        }else if(recv_data[1] == (char)0x33){
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText(MACHINE_MALFUNCTION_TEXT);
            msgbox.exec();
          }else {
            QSettings communication_err_data("shanghaikairen","communication_error");
            communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);
            QMessageBox msgbox;
            msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
            msgbox.setText(TRANSMIT_DATA_ERROR);
            msgbox.setInformativeText(recv_data);
            msgbox.exec();
          }
        on_b_return_clicked();
    }
    if(DISPLAY_ALTER_PASSWD != flag)return;
    if(tmppasswd.size() != 6)return;

    //下面是修改密码的代码
    mysettings.setValue("passwd",tmppasswd);
    qDebug() << mysettings.value("passwd").toString();

    QMessageBox msgbox;
    msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    msgbox.setText("修改密码成功，请牢记此密码" + tmppasswd);
    msgbox.exec();

    on_b_return_clicked();
}

void passwd::alterpasswd_show(int index){
    //提供给修改计数管，光管的界面
    if(index == SETUP_COUNT_VOLTAGE){
        ui->label_title->setText(tr("设定计数管高压"));

        ui->label_caution->setText(QString(tr("请输入电压(电压范围在%1-%2V)")).arg(COUNT_VOLTAGE_MIN).arg(COUNT_VOLTAGE_MAX));
        ui->label_unit->setText("V");
        ui->lineEdit->setText(mysettings.value("count_voltage").toString());
    }else if(index == SETUP_LIGHT_VOLTAGE){
        ui->label_title->setText(tr("设定光管高压"));
        ui->label_caution->setText(tr("请输入电压"));
        ui->label_unit->setText("V");
        ui->lineEdit->setText(mysettings.value("light_voltage").toString());
    }else if(index == SETUP_LIGHT_ELECTRICITY){
        ui->label_title->setText(tr("设定光管电流"));
        ui->label_caution->setText(tr("请输入电流"));
        ui->label_unit->setText("μA");
        ui->lineEdit->setText(mysettings.value("light_electricty").toString());
    }else if(index == DISPLAY_ALTER_PASSWD){
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
  if(count_voltage > COUNT_VOLTAGE_MAX || count_voltage < COUNT_VOLTAGE_MIN){
      return 1;
    }
  ui->lineEdit->setText(QString::number(count_voltage));
  flag = SETUP_COUNT_VOLTAGE;
  on_b_ok_clicked();
  return ALL_RIGHT;
}

int passwd::stop_all_measurement(){
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(com::fd,TCIOFLUSH);
  if (com::transmit(STOP_ORDER,3) <= 0){
      QMessageBox msgbox;
      msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.exec();
      return ERRNO_COMMUNICATION_1;
    }
  QString recv_data = com::receive(1);
  if(recv_data == NULL){
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
      msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
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
