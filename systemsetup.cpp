#include <QMessageBox>

#include "systemsetup.h"
#include "ui_systemsetup.h"
#include "passwd.h"

systemsetup::systemsetup(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::systemsetup)
{
  ui->setupUi(this);

  setdt = new setupdatetime();
  calibrate = new calibration();
  modify_kb = new modify_kb_value();
  count_mea = new countingMeasurement();
  specture_mea = new spectrummeasurement();
  communicate_and_help = new communication_help();
  //tmppwd = new passwd();

  //发送一个信号使：当按任何测量的开始键  时要发送一个  10分钟自动测量的停止按钮
  connect(specture_mea,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));
  connect(count_mea,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));
  connect(calibrate,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));

  //当在主界面下按移动滑板使能谱和计数的按钮也改变状态
  connect(this,SIGNAL(change_sliding_disabled(bool,bool)),count_mea,SLOT(set_sliding_disabled(bool,bool)));
  connect(this,SIGNAL(change_sliding_disabled(bool,bool)),specture_mea,SLOT(set_sliding_disabled(bool,bool)));

  //当按能谱和计数测量的移动滑板时改变另外两个移动滑板的状态
  connect(specture_mea,SIGNAL(transmit_move_sliding(bool,bool)),this,SIGNAL(change_widget_sliding(bool,bool)));
  connect(specture_mea,SIGNAL(transmit_move_sliding(bool,bool)),count_mea,SLOT(set_sliding_disabled(bool,bool)));
  connect(count_mea,SIGNAL(transmit_move_sliding(bool,bool)),this,SIGNAL(change_widget_sliding(bool,bool)));
  connect(count_mea,SIGNAL(transmit_move_sliding(bool,bool)),specture_mea,SLOT(set_sliding_disabled(bool,bool)));

  INIT_LABEL_SIZE_FONT;
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
  ui->label->setObjectName("title");
}

systemsetup::~systemsetup()
{
  //delete tmppwd;
  delete communicate_and_help;
  delete modify_kb;
  delete calibrate;
  delete setdt;
  delete ui;
}

void systemsetup::on_pushButton_2_clicked()
{
  emit show_alterpasswd(SETUP_ALTER_PASSWD);
}

void systemsetup::on_pushButton_clicked()
{
  //setup date and time
  setdt->show_and_refresh();
}

void systemsetup::on_pushButton_6_clicked()
{
  this->close();
}

void systemsetup::on_pushButton_4_clicked()
{
  //calibration button slot
  calibrate->showFullScreen();

}

void systemsetup::on_pushButton_3_clicked()
{
  modify_kb->show_and_refresh();
}

void systemsetup::on_pushButton_7_clicked()
{
  count_mea->showFullScreen();
}

void systemsetup::on_pushButton_9_clicked()
{
  specture_mea->showFullScreen();
}

void systemsetup::on_pushButton_5_clicked()
{
  communicate_and_help->showFullScreen();
}
