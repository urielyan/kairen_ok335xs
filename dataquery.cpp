#include "dataquery.h"
#include "ui_dataquery.h"
#include"global.h"
#include <stdio.h>
#include "samplemeasurementquery.h"

dataquery::dataquery(QWidget *parent):
    QWidget(parent),
    ui(new Ui::dataquery)
{
  PRINT_DEBUG_INFOR;
    ui->setupUi(this);
    showcountdataquery = new countdataquery();
    csquery = new caibrateresultquery();
    samplemeasurement_query = new samplemeasurementquery();
    p_sqlDataQuery = new WinSqlDataQuery();
    //printf("%s\n",__FUNCTION__);

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      }
    ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
    ui->label->setObjectName("title");
    PRINT_DEBUG_INFOR;
}

dataquery::~dataquery()
{
    delete p_sqlDataQuery;
    delete samplemeasurement_query;
    delete csquery;
    delete showcountdataquery;
    delete ui;
}

void dataquery::on_pushButton_2_clicked()
{
    //count data query
    showcountdataquery->show_and_refresh();
}

void dataquery::on_pushButton_4_clicked()
{
    this->close();
}

void dataquery::on_pushButton_3_clicked()
{
    //标定结果查询
    csquery->show_and_update();
}

void dataquery::on_pushButton_clicked()
{
#ifdef FRIENDLYARM_TINY210_NOSQL
  samplemeasurement_query->show_and_refresh();
#else
  p_sqlDataQuery->show_and_refresh();
#endif
}
