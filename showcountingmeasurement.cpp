#include "showcountingmeasurement.h"
#include "ui_showcountingmeasurement.h"
#include "global.h"
#include "issample.h"
#include "ispreheat.h"

#include <QByteArray>
#include <math.h>
//#include "countingmeasurement.h"

//QSettings count_data("shanghaikairen","count_data");

showcountingmeasurement::showcountingmeasurement(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::showcountingmeasurement)
{
  ui->setupUi(this);

  ui->tableWidget->setColumnWidth(0,this->size().width()/3);
  for(int i = 0;i < ui->tableWidget->rowCount();i++){
      ui->tableWidget->setRowHeight(i,this->size().height()/8);
    }
  ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("计数数据"));

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->tableWidget->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
}

showcountingmeasurement::~showcountingmeasurement()
{
  delete ui;
}
extern int measurement_flag;
void showcountingmeasurement::adddata(int count,QString data){
#if 1
  QSettings count_data("shanghaikairen","count_data");
  //if(count == 1) clearall();
  data.remove(0,2);
  QTableWidgetItem *tmpitem = new QTableWidgetItem(tr("%1").arg(data));
  tmpitem->setTextAlignment(Qt::AlignHCenter);
  ui->tableWidget->setItem(0,count - 1,tmpitem);
  if(count == 11){
      int i = 0;
      double y = 0;
      double sumsub = 0;
      long long sum = 0;
      for(i = 0;i < 11;i++){
          sum += ui->tableWidget->item(i,0)->text().toInt();
        }
      for(i = 0;i < 10; i++){
          sumsub += pow(sum / 11 - ui->tableWidget->item(i,0)->text().toInt(),2);
        }
      y = pow((sumsub / (double)(sum/11)) * (double)(TURN_SECONDS/10),0.5);
      int tmp_count = count_data.value("count_count").toInt() + 1;
      if(tmp_count > COUNT_MEASUREMENT_MOST_STORAGE) tmp_count = 1;
      QString tmp_y = QString::number(y,'f',4);
      ui->label_data->setText(QDateTime::currentDateTime().toString("MM月dd日 hh:mm"));
      ui->label_x->setText(QString("%1").arg(sum/11));
      ui->label_y->setText(QString("%1").arg(tmp_y));

      if(issample::global_is_sample == WAIT_BE_LOCATION || ui->widget_2->global_ispreheat >=  2){
          return;
        }
      count_data.setValue(QString("count_data_%1").arg(tmp_count),QString("%1").arg(sum/11) + ";"\
                          + tmp_y + ";" +QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
      count_data.setValue("count_count",tmp_count);
    }
#endif
}
void showcountingmeasurement::clearall(){
  ui->tableWidget->clearContents();
  ui->label_data->clear();
  ui->label_x->clear();
  ui->label_y->clear();
}

float showcountingmeasurement::get_y(){
  if(ui->label_y->text() != NULL)
    return ui->label_y->text().toFloat();
  return 0.0;
}
float showcountingmeasurement::get_average_x()
{
  if(ui->label_x->text() != NULL)
    return ui->label_x->text().toFloat();
  return 0.0;
}
void showcountingmeasurement::on_pushButton_clicked()
{
  this->close();
}
