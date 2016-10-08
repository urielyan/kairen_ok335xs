#include "query_change_voltage.h"
#include "ui_query_change_voltage.h"
#include "global.h"
#include "datasave.h"

query_change_voltage::query_change_voltage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::query_change_voltage)
{
  ui->setupUi(this);

  ui->tableWidget->setColumnWidth(0,this->width()/3);
  ui->tableWidget->setColumnWidth(1,this->width()/4);
  ui->tableWidget->setColumnWidth(2,this->width()/4);

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
  ui->tableWidget->setFont(QFont(FONT_NAME, FONT_SIZE,QFont::Normal));
  ui->label->setObjectName("title");
}

query_change_voltage::~query_change_voltage()
{
  delete ui;
}

void query_change_voltage::show_and_refresh(){
  int change_count = ErrorCountSave::instance()->value(MYSETTINGS_CHANGE_COUNT_VOLTAGE_COUNT).toInt() - 1;
  ui->tableWidget->setRowCount(change_count);
  ui->tableWidget->setColumnWidth(0,this->width()/3);
  ui->tableWidget->setColumnWidth(1,this->width()/4);
  ui->tableWidget->setColumnWidth(2,this->width()/4);
  for(int i =  0;i < change_count;i++){
      QStringList record = ErrorCountSave::instance()->value(MYSETTINGS_CHANGE_COUNT_VOLTAGE(i+1)).toString().split(";");
      if(record.size() != 3){
          return;
        }
      QTableWidgetItem *item1 = new QTableWidgetItem(record[0]);
      item1->setTextAlignment(Qt::AlignCenter);
      ui->tableWidget->setItem(i,0,item1);

      QTableWidgetItem *item2 = new QTableWidgetItem(record[1]);
      item2->setTextAlignment(Qt::AlignCenter);
      ui->tableWidget->setItem(i,1,item2);

      QTableWidgetItem *item3 = new QTableWidgetItem(record[2]);
      item3->setTextAlignment(Qt::AlignCenter);
      ui->tableWidget->setItem(i,2,item3);
    }
  this->showFullScreen();
}
void query_change_voltage::on_pushButton_clicked()
{
  ui->tableWidget->clear();
  this->close();
}
