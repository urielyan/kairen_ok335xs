
#include "hide_system.h"
#include "ui_hide_system.h"
#include "global.h"
#include "passwd.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>

hide_system::hide_system(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::hide_system)
{
  ui->setupUi(this);
  proportion_widget = new proportion();

  query_change_vol = new query_change_voltage();

  timer = new QTimer();
  connect(timer, SIGNAL(timeout()), this, SLOT(time_use_count()));
  timer->start(1000);

  input_time = new input_machine_use_time();

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));
  ui->label->setObjectName("title");
}

hide_system::~hide_system()
{
  delete query_change_vol;
  delete ui;
  delete timer;
  delete input_time;
}

void hide_system::on_pushButton_clicked()
{
  this->close();
}

void hide_system::on_b_count_voltage_clicked()
{
  emit change_voltage_electricity(SETUP_COUNT_VOLTAGE);
}

void hide_system::on_b_light_voltage_clicked()
{
  emit change_voltage_electricity(SETUP_LIGHT_VOLTAGE);
}

void hide_system::on_b_light_electricity_clicked()
{
  emit change_voltage_electricity(SETUP_LIGHT_ELECTRICITY);
}

void hide_system::on_b_clear_clicked()
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setStandardButtons(QMessageBox::Cancel|QMessageBox::Yes);
  msgbox.setText("清除数据？");
  int result_box = msgbox.exec();
  if(result_box == QMessageBox::Yes){
      QSqlQuery myquery;
      bool ok = myquery.exec("delete from sample_data where 1;");
      if(ok == false){
          QMessageBox box1;
          box1.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          box1.setText("未清除含量测量数据");
          box1.exec();
          return;
        }
      QMessageBox box2;
      box2.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      box2.setText("已经清除完含量数据");
      box2.exec();
    }else if(result_box == QMessageBox::Cancel){
    }
}

void hide_system::on_pushButton_2_clicked()
{
  proportion_widget->showAndInit();
}

void  hide_system::time_use_count(){
  //一秒进入此函数一次，检查使用日期和最大使用日期。当最大使用日期为0时，仪器可以无限使用。当使用时间大于等于最大使用时间时仪器不能进行除了解码之外的任何操作

  used_time = mysettings.value("machine_used_time").toInt();
  most_use_time = mysettings.value("most_use_time").toInt();

  if(most_use_time == 0){
      //timer->stop();
      return;
    }
  if(used_time >= most_use_time  * 30 * 24 * 60){//* 30 * 24 * 60
    input_time->input_decode_serial();
    }
  static int i = 0;
  i += 1;

  if(i < 60){
      return;
    }

  mysettings.setValue("machine_used_time",used_time + 1);
  used_time += 1;
}
void hide_system::on_b_input_serial_clicked()
{
  input_time->input_product_serial();
  //exit(0);
}

void hide_system::on_pushButton_3_clicked()
{
  query_change_vol->show_and_refresh();
}
