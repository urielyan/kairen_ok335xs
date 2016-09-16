#include "caibrateresultquery.h"
#include "ui_caibrateresultquery.h"
#include <QDebug>
#include "global.h"
#include "datasave.h"

#define MYSETTINGS_COUNT "calibratemeasurement_count_record"
#define MYSETTINGS_RESULT "calibration_results_in_result_"
#define MYSETTINGS_DATA "calibration_results_in_data_"

caibrateresultquery::caibrateresultquery(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::caibrateresultquery)
{
  ui->setupUi(this);
  //得到一个下一次标定会记录在哪的数，所以减一就是最新的数
  display_number = DataSave::instance()->value(MYSETTINGS_COUNT).toInt() - 1;
  update_page(display_number);



#ifdef FORLIN_OK335XS
  //调整tablewidget的大小以适应屏幕。
  for(int i = 0; i < ui->tableWidget->columnCount();i++){
      ui->tableWidget->setColumnWidth(i, DESKTOP_WIDTH / 9);
    }

  for(int i = 0; i < 3; i++)
    {
      ui->tableWidget->setRowHeight(i, DESKTOP_HEIGHT / 10);
    }
  INIT_LABEL_SIZE_FONT;

//  for(int i = 0; i < ui->tableWidget->columnCount();i++){
//      ui->tableWidget->setColumnWidth(i,FONT_SIZE * 5);
//    }

//  ui->tableWidget->setRowHeight(0,FONT_SIZE * 2);
//  ui->tableWidget->setRowHeight(1,FONT_SIZE * 2);
//  ui->tableWidget->setRowHeight(2,FONT_SIZE * 2);
//  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
//  QList<QLabel *> label_all = this->findChildren<QLabel *>();
//  for(int i = 0;i <label_all.size();i++){
//      label_all[i]->setFont(QFont(FONT_NAME, FONT_SIZE/5*4,QFont::Normal));
//    }
#endif
#ifdef FRIENDLYARM_TINY210
  //调整tablewidget的大小以适应屏幕。
  for(int i = 0; i < ui->tableWidget->columnCount();i++){
      ui->tableWidget->setColumnWidth(i, DESKTOP_WIDTH / 9);
    }

  for(int i = 0; i < 3; i++)
    {
      ui->tableWidget->setRowHeight(i, DESKTOP_HEIGHT / 10);
    }
  INIT_LABEL_SIZE_FONT;
#endif

  QList<QPushButton *> buttonlist = this->findChildren<QPushButton *>();
  for (int i = 0; i < buttonlist.count(); ++i) {
      buttonlist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->tableWidget->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
  ui->label->setObjectName("title");
}

caibrateresultquery::~caibrateresultquery()
{
  delete ui;
}

void caibrateresultquery::update_page(int count){
  //init
  ui->tableWidget->clearContents();

  QString tmp_result,tmp_data;
  QStringList tmp_result_list,tmp_data_list,tmp_data_list2;

  //result:
  tmp_result = DataSave::instance()->value(QString(MYSETTINGS_RESULT) + QString::number(count)).toString();
  tmp_result_list = tmp_result.split(";");
  if(tmp_result_list.size() != 4) return;
  ui->label_datetime->setText(QString(tr("标定测量时间：")) + tmp_result_list[0]);
  ui->label_work_curve->setText(tmp_result_list[1]);
  ui->label_kbr->setText(QString(tr("标定结果：")) + tmp_result_list[2]);
  ui->label_number->setText(tmp_result_list[3]);

  //data:tablewidget
  tmp_data = DataSave::instance()->value(QString(MYSETTINGS_DATA) + QString::number(count)).toString();
  tmp_data_list = tmp_data.split(";");
  //判断数据是否会出错，即此标定数据的个数与此标定结果的相应数据是否对应

  int i,j;

  for(i = 0;i < tmp_data_list.size()  ;i++){
      tmp_data_list2 = tmp_data_list[i].split("/");
      if(tmp_data_list2.size() != 3) continue;
      for(j = 0 ; j < 3 ; j++){
          QTableWidgetItem *item = new QTableWidgetItem(tmp_data_list2[j]);
          item->setTextAlignment(Qt::AlignCenter);
          ui->tableWidget->setItem(j,i,item);
        }
    }
}

void caibrateresultquery::show_and_update(){
  display_number = DataSave::instance()->value(MYSETTINGS_COUNT).toInt() - 1;
  update_page(display_number);
  this->showFullScreen();
}
void caibrateresultquery::on_pushButton_clicked()
{
  //return button;
  this->close();
}

void caibrateresultquery::on_pushButton_2_clicked()
{
  //next button;
  if(display_number <= 1)
    display_number = CALIBRATE_RESULT_MAX_RECORD;
  update_page(--display_number);
}
