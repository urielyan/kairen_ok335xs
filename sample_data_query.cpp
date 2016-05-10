#include "sample_data_query.h"
#include "ui_sample_data_query.h"
#include "global.h"

#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>

#define QUERY_BY_PEOPLE 1
#define QUERY_BY_SAMPLE 2

WinSqlDataQuery::WinSqlDataQuery(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::sample_data_query)
{
  //printf("in...%s\n",__FUNCTION__);
  ui->setupUi(this);

  /*
 * 判断当前是什么查询
*/
  query_flag = 0;

  model = new QSqlTableModel();
  input_serial = new input_person_sampleSerial();

  #if 1
  QScrollBar *verticalbar;
  verticalbar = new QScrollBar();
  //verticalbar->setFixedWidth(this->width()/2);
  verticalbar->setStyleSheet("width:40px");
  ui->tableView->setVerticalScrollBar(verticalbar);

  QScrollBar *horizontalbar;
  horizontalbar = new QScrollBar();
  //horizontalbar->setFixedHeight(this->width()/10);
  horizontalbar->setStyleSheet("height:40px");
  ui->tableView->setHorizontalScrollBar(horizontalbar);
#endif

  on_b_datetime_disorder_clicked();
  //printf("%s\n",__FUNCTION__);

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QPushButton *> labellist = this->findChildren<QPushButton *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->tableView->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
  //ui->label->setFont(QFont(FONT_NAME, FONT_SIZE/2,QFont::Normal));
  //ui->label->setObjectName("title");
}

WinSqlDataQuery::~WinSqlDataQuery()
{
  delete ui;

  delete model;
  delete input_serial;
}
void WinSqlDataQuery::initTableview(){
  delete model;
  //ui->tableView->setColumnWidth(2,1000);
  model = new QSqlTableModel();
  model->setTable("sample_data");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setHeaderData(0, Qt::Horizontal, tr("人员编号"));
  model->setHeaderData(1, Qt::Horizontal, tr("样品编号"));
  model->setHeaderData(2, Qt::Horizontal, tr("测量日期"));
  model->setHeaderData(3, Qt::Horizontal, tr("工作曲线"));
  model->setHeaderData(4, Qt::Horizontal, tr("测量时间"));
  model->setHeaderData(5, Qt::Horizontal, tr("重复次数"));
  model->setHeaderData(6, Qt::Horizontal, tr("平均值"));
  model->setHeaderData(7, Qt::Horizontal, tr("标准偏差"));
  model->setHeaderData(8, Qt::Horizontal, tr("是否自动"));
  model->setHeaderData(9, Qt::Horizontal, tr("测量系数"));

  //ui->tableView->setColumnWidth(2,1000);
#define SUIT_DIVIDE 6
  ui->tableView->setColumnWidth(0,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(1,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(2,this->width() /3);
  ui->tableView->setColumnWidth(3,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(4,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(5,this->width() /SUIT_DIVIDE);
  //ui->tableView->setColumnWidth(6,this->width() /SUIT_DIVIDE);
  //ui->tableView->setColumnWidth(7,this->width() /SUIT_DIVIDE);

  //ui->tableView->setRowHeight(0,this->height()/2);
}

void WinSqlDataQuery::show_and_refresh(){

  initTableview();
  model->select();

  ui->tableView->setModel(model);
  ui->tableView->setColumnWidth(0,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(1,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(2,this->width() /3);
  ui->tableView->setColumnWidth(3,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(4,this->width() /SUIT_DIVIDE);
  ui->tableView->setColumnWidth(5,this->width() /SUIT_DIVIDE);
  this->showFullScreen();
}

void WinSqlDataQuery::wait_input_result(QString recv_data){
  if(recv_data == NULL)return;

  initTableview();
  if(query_flag == QUERY_BY_PEOPLE){
      model->setFilter("people_id=" + QString("\"") + recv_data + "\"");
      //model->setFilter("people_id=" + QString::number(recv_data.toInt()));
    }else if(query_flag == QUERY_BY_SAMPLE){
      model->setFilter("sample_serial="+ QString("\"") + recv_data + "\"");
    }
  model->select();
  ui->tableView->setModel(model);
}

void WinSqlDataQuery::on_pushButton_clicked()
{
  //按人员编号查询
  query_flag = QUERY_BY_PEOPLE;
  connect(input_serial,SIGNAL(transmit_data(QString)),this,SLOT(wait_input_result(QString)));
  input_serial->just_show_people();
}


void WinSqlDataQuery::on_b_datetime_clicked()
{
  initTableview();
  model->setSort(2,Qt::AscendingOrder);
  model->select();
  ui->tableView->setModel(model);
}

void WinSqlDataQuery::on_b_datetime_disorder_clicked()
{
  initTableview();
  model->setSort(2,Qt::DescendingOrder);
  model->select();
  ui->tableView->setModel(model);
}

void WinSqlDataQuery::on_b_sample_clicked()
{
  query_flag = QUERY_BY_SAMPLE;
  connect(input_serial,SIGNAL(transmit_data(QString)),this,SLOT(wait_input_result(QString)));
  input_serial->just_show_sample();
}


void WinSqlDataQuery::on_pushButton_3_clicked()
{
  disconnect(input_serial,SIGNAL(transmit_data(QString)),this,SLOT(wait_input_result(QString)));
  disconnect(input_serial,SIGNAL(transmit_data(QString)),this,SLOT(wait_input_result(QString)));
  this->close();
}

void WinSqlDataQuery::on_tableView_clicked(const QModelIndex &index)
{
  if(index.column() != 2){
      return;
    }
  QStringList strlist ;
  QStringList table_schema;
  strlist << "人员编号" << "样品编号" << "测量日期" << "工作曲线" << "测量时间" << "重复次数" << "平均值" << "标准偏差" << "是否自动" << "测量系数";
  table_schema <<"people_id" << "sample_serial" << "date_time" << "work_curve" << "measurement_time" << "repeat_time" << "deviation" << "is_auto" << "current_coefficient";
  QMessageBox msgbox;
  msgbox.setFixedHeight(DESKTOP_HEIGHT / 3 * 2);
  msgbox.setFixedWidth(DESKTOP_WIDTH / 6 * 5);
  QString query_str;
  //query_str= QString("SELECT * FROM sample_data WHERE ").append(table_schema[index.column()].append("=").append("\"").append(index.data().toString()).append("\"").append(";");
  //query_str = QString("SELECT * FROM sample_data WHERE ").append(table_schema[index.column()]).append("=").append("\"").append(index.data().toString()).append("\"").append(";");
  query_str = QString("SELECT * FROM sample_data WHERE date_time = ").append("\"").append(index.data().toString()).append("\"").append(";");
  QSqlQuery query;
  bool ok = query.exec(query_str);
  if(ok == true ){
      if(query.next()) {
          QString msgstr;
          for(int i = 0;i < 10;i++){
              msgstr += strlist[i];
              msgstr += ":";
              msgstr += query.value(i).toString();
              msgstr += "\n";
            }
          //msgbox.setStyleSheet("font-size:16px");
          msgbox.setText(msgstr);
          msgbox.exec();
        }
    }
}
