#include "sample_data_query.h"
#include "ui_sample_data_query.h"
#include "global.h"
#include "database.h"

#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QScrollBar>
#include <QButtonGroup>

#define QUERY_BY_PEOPLE 1
#define QUERY_BY_SAMPLE 2

WinSqlDataQuery::WinSqlDataQuery(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::sample_data_query)
{
  //printf("in...%s\n",__FUNCTION__);
  ui->setupUi(this);



  query_flag = 0; //判断当前是什么查询

  m_model = new QSqlTableModel(this, Database::instance()->getDb());
  m_model->setTable("sample_data");
  input_serial = input_person_sampleSerial::instance();

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

  INIT_LABEL_SIZE_FONT;

  ui->tableView->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
}

WinSqlDataQuery::~WinSqlDataQuery()
{
  delete ui;

  delete m_model;
  delete input_serial;
}
void WinSqlDataQuery::initTableview(){
  //delete model;
  //ui->tableView->setColumnWidth(2,1000);
  //model = new QSqlTableModel();
 // model->setTable("sample_data");
ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_model->setHeaderData(0, Qt::Horizontal, tr("人员编号"));
  m_model->setHeaderData(1, Qt::Horizontal, tr("样品编号"));
  m_model->setHeaderData(2, Qt::Horizontal, tr("测量日期"));
  m_model->setHeaderData(3, Qt::Horizontal, tr("工作曲线"));
  m_model->setHeaderData(4, Qt::Horizontal, tr("测量时间"));
  m_model->setHeaderData(5, Qt::Horizontal, tr("重复次数"));
  m_model->setHeaderData(6, Qt::Horizontal, tr("平均值"));
  m_model->setHeaderData(7, Qt::Horizontal, tr("标准偏差"));
  m_model->setHeaderData(8, Qt::Horizontal, tr("是否自动"));
  m_model->setHeaderData(9, Qt::Horizontal, tr("测量系数"));

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

#ifdef FRIENDLYARM_TINY210
  ui->tableView->setFont(QFont(FONT_NAME, FONT_SIZE*5/6,QFont::Normal));
#endif

#ifdef FORLIN_OK335XS
  ui->tableView->setFont(QFont(FONT_NAME, FONT_SIZE*3/4, QFont::Normal));
#endif
}

void WinSqlDataQuery::show_and_refresh(){

  initTableview();
  m_model->select();

  ui->tableView->setModel(m_model);
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
      m_model->setFilter("people_id=" + QString("\"") + recv_data + "\"");
      //model->setFilter("people_id=" + QString::number(recv_data.toInt()));
    }else if(query_flag == QUERY_BY_SAMPLE){
      m_model->setFilter("sample_serial="+ QString("\"") + recv_data + "\"");
    }
  m_model->select();
  ui->tableView->setModel(m_model);
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
  m_model->setSort(2,Qt::AscendingOrder);
  m_model->select();
  ui->tableView->setModel(m_model);
}

void WinSqlDataQuery::on_b_datetime_disorder_clicked()
{
  initTableview();
  m_model->setSort(2,Qt::DescendingOrder);
  m_model->select();
  ui->tableView->setModel(m_model);
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
          WinSpecifyIndexDialog dialog(index, m_model);
          dialog.exec();
}



#define DISPLAY_COUNT  9
WinSpecifyIndexDialog::WinSpecifyIndexDialog(const QModelIndex &index, QSqlTableModel *model, QWidget *parent)
  : QDialog(parent)
{
  m_index = index;
  m_model = model;

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QHBoxLayout *labelLayout = new QHBoxLayout;
  QVBoxLayout *titleLayout = new QVBoxLayout;
  QVBoxLayout *valueLayout = new QVBoxLayout;
  QVBoxLayout *unitLayout = new QVBoxLayout;

  QStringList titleList;
  titleList << tr("人员编号:") << tr("样品编号:") << tr("测量日期:") << tr("工作曲线:") << tr("测量时间:") << tr("重复次数:") << tr("硫含量平均值:") << tr("标准偏差:") <<  tr("系数:");

  QStringList unitList;
  unitList << "" << "" << "" << "" << tr("秒") << tr("次") << "%(m/m)" << "%(m/m" <<  "";

  QLabel *labelTitle[DISPLAY_COUNT];
  QLabel *labelValue[DISPLAY_COUNT];
  QLabel *labelUnit[DISPLAY_COUNT];
  for (int i = 0; i < DISPLAY_COUNT; ++i) {
      labelTitle[i] = new QLabel(titleList.value(i), this);
      titleLayout->addWidget(labelTitle[i]);

      labelValue[i] = new QLabel(this);
      valueLayout->addWidget(labelValue[i]);
      m_labelList.append(labelValue[i]);

      if(unitList.size() != 0)
        {
          labelUnit[i] = new QLabel(unitList.value(i), this);
          labelUnit[i]->setAlignment(Qt::AlignCenter);
          unitLayout->addWidget(labelUnit[i]);
        }
    }
  labelLayout->addLayout(titleLayout);
  labelLayout->addLayout(valueLayout);
  labelLayout->addLayout(unitLayout);

  QPushButton *returnButton = new QPushButton(tr("返回"), this);
  connect(returnButton, SIGNAL(clicked()), this, SLOT(close()));

  QPushButton *nextButton = new QPushButton(tr("下一个"), this);
  connect(nextButton, SIGNAL(clicked()), this, SLOT(slotNextButtonClicked()));

  QPushButton *previousButton = new QPushButton(tr("前一个"), this);
  connect(previousButton, SIGNAL(clicked()), this, SLOT(slotPreviousButtonClicked()));

  QHBoxLayout *hboxLayout = new QHBoxLayout();
  hboxLayout->addWidget(previousButton);
  hboxLayout->addWidget(returnButton);
  hboxLayout->addWidget(nextButton);

  mainLayout->addLayout(labelLayout);
  mainLayout->addLayout(hboxLayout);
  init();
}

void WinSpecifyIndexDialog::slotNextButtonClicked()
{
  if(m_index.row() >= m_model->rowCount() - 1)
    {
      return;
    }
  m_index =  m_model->index(m_index.row() + 1, 2);
  init();
}

void WinSpecifyIndexDialog::slotPreviousButtonClicked()
{
  if(m_index.row() == 0)
    {
      return;
    }
  m_index =  m_model->index(m_index.row() - 1, 2);
  init();
}

void WinSpecifyIndexDialog::init()
{
//  QStringList table_schema;
//  table_schema <<"people_id" << "sample_serial" << "date_time" << "work_curve" << "measurement_time"
//              << "repeat_time" << "deviation" << "is_auto" << "current_coefficient";
  QString query_str;
  QStringList valueList;
  if(m_index.column() != 2)
    {
      m_index = m_model->index(m_index.row() , 2);
    }
  query_str = QString("SELECT * FROM sample_data WHERE date_time=\"%1\";")
      .arg(m_index.data().toString());
  QSqlQuery query(Database::instance()->getDb());
  bool ok = query.exec(query_str);
  if (ok == true ){
      if(query.next()) {
          for (int i = 0; i < 10; i++){
              QString msgstr = query.value(i).toString();
              valueList.append(msgstr);
            }
          valueList.removeAt(8);
          initData(valueList);
        }
  }
}

void WinSpecifyIndexDialog::initData(QStringList valueList)
{
  Q_ASSERT(valueList.size() == DISPLAY_COUNT);
  for (int i = 0; i < DISPLAY_COUNT; i ++)
    {
      m_labelList.at(i)->setText(valueList.at(i));
    }
}

