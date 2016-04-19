
#include "spectrummeasurement.h"
#include "ui_spectrummeasurement.h"
#include "widget.h"
#include "global.h"
#include "printer.h"

#include <QMessageBox>
#include <QString>
#include <QTextEdit>
extern int measurement_flag;

static int specture_array_data[SPECTRUM_PAINTER_WIDTH] = {
  0,100,200,300,592,
  592,641,656,720,720,
  769,865,944,1089,1281,
  1360,1360,1473,1360,1296,
  1057,816,609,417,321,
  257,272,400,577,944,
  1498,2000,2576,2721,2192,
  1616,939,400,161,80,33,16,33,48,48,48,48,0};
spectrummeasurement::spectrummeasurement(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::spectrummeasurement)
{
  ui->setupUi(this);
  row = 0;
  column = 0;
  /*
        if flag == 3 represent user need clear all item and set row,column to start;
        if flag == 2 represent user just press start all the time;
    */
  flag = 0;
  /* try_inspectoscope_flag judge whether current measurement is inspectoscope-self:
            if try_inspectoscope_flag is 1 represent doing inspectoscope-self
            else hand-inspectoscope;
    */
  try_inspectoscope_flag = 0;
  //timer setup

  countmeas = new countingMeasurement();
  timer = new QTimer();
  connect(timer,SIGNAL(timeout()),this,SLOT(changetable()));
  spectrum_com = new com();
  ui->tableWidget->clearContents();
  for(int i = 0;i < ui->tableWidget->rowCount();i++){
      ui->tableWidget->setRowHeight(i,FONT_SIZE - 2);
    }
  for(int i = 0;i < ui->tableWidget->columnCount();i++){
      ui->tableWidget->setColumnWidth(i,FONT_SIZE * 4 - 10);
    }

  painter_result = new painter_histogram();

  this->setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
    }
  ui->tableWidget->setFont(QFont("wenquanyi", FONT_SIZE/5*4 ,QFont::Normal));
  ui->label->setFont(QFont("wenquanyi", FONT_SIZE*2,QFont::Normal));

    set_sliding_disabled(true,false);
    ui->pushButton_6->setDisabled(true);
}

spectrummeasurement::~spectrummeasurement()
{
  delete painter_result;
  delete spectrum_com;
  delete timer;
  delete countmeas;
  delete ui;
}

void spectrummeasurement::try_inspectoscope(){
  on_pushButton_clicked();
  try_inspectoscope_flag = 1;
}

static bool turn_inspectoscope = true;//实现一秒钟发送一次自检数据
static int last_row = 0,last_column = 0;
void spectrummeasurement::changetable(){
#if 1
  //实现一秒钟发送一次自检数据
  if(turn_inspectoscope == true){
      emit transmit(80 - 2 * row - column * 10 - 1);
      turn_inspectoscope = false;
      return;
    }else {
      turn_inspectoscope = true;
    }
#endif

  //tablewidget setup
  QString recv_data = com::receive(3);
  if (NULL == recv_data ){
      QMessageBox msgbox;
      msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
      msgbox.setText("未接收到数据，已停止测量！");
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_1",communication_err_data.value("com_err_1").toInt() + 1);
      on_pushButton_2_clicked();
      msgbox.exec();
      return;
    }else if(recv_data[0] != (char)0x01){
      QMessageBox msgbox;
      msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
      msgbox.setText("接收数据有误，已停止测量！");
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_1",communication_err_data.value("com_err_1").toInt() + 1);
      on_pushButton_2_clicked();
      msgbox.exec();
      return;
    }

//  static QString max_data = "00000";
  if(row == 0 && column == 0){
      //重新开始时初始化
      max_data = "00000";
      last_row = 0,last_column = 0;
      for(int i = 0; i < SPECTRUM_PAINTER_WIDTH;i++){
          specture_array_data[i] = 0;
        }
    }
  specture_array_data[0] = 10;
  specture_array_data[1] = 20;
  specture_array_data[2] = 30;
  specture_array_data[3] = 40;
  specture_array_data[SPECTRUM_PAINTER_WIDTH -1] = 20;
  specture_array_data[SPECTRUM_PAINTER_WIDTH -2] = 80;
  specture_array_data[SPECTRUM_PAINTER_WIDTH -3] = 160;
  specture_array_data[SPECTRUM_PAINTER_WIDTH -4] = 300;

  recv_data.remove(0,1);
  QTableWidgetItem *item1 = new QTableWidgetItem(tr("%1.%2").arg(recv_data[0]).arg(recv_data[1]));
  item1->setTextAlignment(Qt::AlignCenter);
  recv_data.remove(0,2);
  QTableWidgetItem *item2 = new QTableWidgetItem(tr("%1").arg(recv_data));
  item2->setTextAlignment(Qt::AlignCenter);

  specture_array_data[row + column*10/2 + 4] = recv_data.toInt();
  painter_result->update();
  painter_result->just_updae();

  ui->tableWidget->setItem(row,column,item1);
  ui->tableWidget->setItem(row,column + 1,item2);
  if((column == 2) || ((column == 0) && (row >= 3) ) ){
      if(QString::compare(recv_data,max_data) > 0 && (column == 0 || column == 2)){
          max_data = recv_data;
          ui->tableWidget->item(row,column)->setBackgroundColor(QColor(255,0,0,180));
          ui->tableWidget->item(row,column + 1)->setBackgroundColor(QColor(255,0,0,180));
          summit_value = row + column*10/2 + 4 + 1;
          if(row != 0 || column != 0){
              ui->tableWidget->item(last_row,last_column)->setBackgroundColor(QColor(255,255,255,255));
              ui->tableWidget->item(last_row,last_column + 1)->setBackgroundColor(QColor(255,255,255,255));
            }
          last_column = column;
          last_row = row;
          max_volumn = "  " + QString::number((double)(row + column*10/2 + 4) / 10);
        }
    }
#if 1
  //在计数后的稳峰测量当到2.4时结束测量
  if((measurement_flag == MEASUREMENT_STEADY_SUMMIT) && (column ==2) && (row == 9)){
      on_pushButton_2_clicked();
      if(last_column != 2 || last_row > 0){
          emit transmit_steady_summit_result(ui->tableWidget->item(last_row,last_column)->text().toDouble());
          return;
        }
      //on_pushButton_2_clicked();
    }
#endif
  emit transmit(80 - 2 * row - column * 10 - 2);
  if(row == 9 && column == 6){
      //is ended
      timer->stop();
      measurement_flag = MEASUREMENT_NOTHING;
      flag = 3;
      row = column = 0;

      disable_pushbutton(false);
      if(1 == try_inspectoscope_flag){//进行自检操作
          if(last_column != 2 || last_row > 2){
              QMessageBox msgbox;
              msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
              msgbox.setText("能谱未通过，需要重新检测么？\n按是重新检测，按否开始计数检测，按取消停止自检");
              msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
              int ret = msgbox.exec();
              if(ret == QMessageBox::Yes){
                  emit transmit(DEFEAT_SPECTOSCOPE);
                  on_pushButton_2_clicked();
                  usleep(100);
                  on_pushButton_clicked();
                  try_inspectoscope_flag = 1;
                  return;
                }else if (ret == QMessageBox::No) {
                  emit transmit(DEFEAT_SPECTOSCOPE);
                  emit transmit_stop_auto_count();
                  try_inspectoscope_flag = 0;
                  connect(countmeas,SIGNAL(transmit_time(int)),this,SLOT(receive_transmit_count_time(int)));
                  connect(countmeas,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));
                  countmeas->try_inspectoscope();
                  return;
                }else if (ret == QMessageBox::Cancel){
                  emit transmit(DEFEAT_SPECTOSCOPE);
                  on_pushButton_2_clicked();
                  return;
                }
            }
#if 1
          emit transmit(SUCCESS_SPECTOSCOPE);
          emit transmit_stop_auto_count();
          //机器正常
          try_inspectoscope_flag = 0;
          connect(countmeas,SIGNAL(transmit_time(int)),this,SLOT(receive_transmit_count_time(int)));
          connect(countmeas,SIGNAL(transmit_stop_auto_count()),this,SIGNAL(transmit_stop_auto_count()));
          countmeas->try_inspectoscope();
          return;
#endif
        }
      printer_result();
      on_pushButton_6_clicked();
      return;
    }
  if(row == 9){
      column +=2;
      row = 0;
      return;
    }
  row += 1;
  //tcflush(com::fd,TCIOFLUSH);
}

void spectrummeasurement::on_pushButton_3_clicked()
{
  //return button
  if(flag == 2){
      return;
    }
  ui->tableWidget->clearContents();
  row = column = 0;
  this->close();
}

void spectrummeasurement::disable_pushbutton(bool value){
  ui->pushButton->setDisabled(value);
  ui->pushButton_3->setDisabled(value);
  ui->pushButton_4->setDisabled(value);
  ui->pushButton_5->setDisabled(value);
  ui->pushButton_6->setDisabled(value);
}
void spectrummeasurement::on_pushButton_clicked()
{
  //printer_result();
  //start button
  if(flag == 3){
      ui->tableWidget->clearContents();
    }else if(flag == 2){
      return;
    }
  //judge whether have any other measurement.
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  usleep(100);
  //开始之前停止任何测量,test communication
  if(on_pushButton_2_clicked() != ALL_RIGHT)return;

  tcflush(com::fd,TCIOFLUSH);
  //开始能谱测量
  if(com::transmit(ACTIVATING_SPECTRUM,3) <= 0){
      QMessageBox msgbox;
      msgbox.setFont(QFont("wenquanyi", FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.setInformativeText("transmit activating");
      msgbox.exec();
      on_pushButton_2_clicked();
      return;
    }
  //usleep(100);
  disable_pushbutton(true);
  timer->start(1000);
  measurement_flag = MEASUREMENT_SPECTRUM;
  tcflush(com::fd,TCIOFLUSH);
  row = column = 0;
  flag = 2;
}

int spectrummeasurement::on_pushButton_2_clicked()
{
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  //stop button
  disable_pushbutton(false);
  try_inspectoscope_flag = 0;
  timer->stop();
  measurement_flag = MEASUREMENT_NOTHING;
  row = column = 0;
  flag = 3;
  turn_inspectoscope = true;
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
int spectrummeasurement::stop_measurement_no_transmit(){
  disable_pushbutton(false);
  try_inspectoscope_flag = 0;
  timer->stop();
  measurement_flag = MEASUREMENT_NOTHING;
  row = column = 0;
  flag = 3;
  turn_inspectoscope = true;
  return ALL_RIGHT;
}

static int move_sliding_count = 0;
static QDateTime last_move_sliding_datetime = QDateTime::currentDateTime();
void spectrummeasurement::on_pushButton_5_clicked()
{
  //移进滑板到待测样
  if(move_sliding_count >= 2){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NOT_ALLOW);
      msgbox.exec();
      return;
    }
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(com::fd,TCIOFLUSH);
  measurement_flag = MEASUREMENT_NOTHING;
  if(com::transmit(IN_SLIDING_PLATE,4) < 0){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.setInformativeText("transmit err");
      msgbox.exec();
      if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) > 5){
          move_sliding_count = 0;
        }
      return;
    }
  QString recv_data = com::receive(SLIDING_PLATE_CHANGE_TIME);
  if(recv_data == NULL){
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);

      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
      msgbox.exec();
    }else if(recv_data[1] == (char)0x32){//recv_data[0] == (char)0x98 &&
      ui->widget->change_label_content(WAIT_BE_LOCATION);
    }else if( recv_data[1] == (char)0x33){//recv_data[0] == (char)0x98 &&
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(MACHINE_MALFUNCTION_TEXT);
      msgbox.exec();
      set_sliding_disabled(false,false);
      emit transmit_move_sliding(false,false);
    }else {
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);

      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(QString(SLIDING_PLATE_NO_CHANGE_TEXT) + ":" + recv_data + "," + QString::number(measurement_flag));
      msgbox.exec();
    }

  set_sliding_disabled(false,true);
  emit transmit_move_sliding(false,true);
  if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) < 5){
      move_sliding_count++;
    }else{
      move_sliding_count = 0;
    }
  last_move_sliding_datetime = QDateTime::currentDateTime();
}

int spectrummeasurement::on_pushButton_4_clicked()
{
  //移出滑板到参考样,
  if(move_sliding_count >= 2){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NOT_ALLOW);
      msgbox.exec();
      if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) > 5){
          move_sliding_count = 0;
        }
      return -2;
    }
  if(measurement_flag != MEASUREMENT_NOTHING){
      emit transmit_stop_auto_count();
    }
  tcflush(com::fd,TCIOFLUSH);
  measurement_flag = MEASUREMENT_NOTHING;
  if(com::transmit(OUT_SLIDING_PLATE,4) < 0){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(TRANSMIT_DATA_ERROR);
      msgbox.exec();
      return -1;
    }
  QString recv_data = com::receive(SLIDING_PLATE_CHANGE_TIME);
  //qDebug() <<recv_data.toLocal8Bit().data();
  if(recv_data == NULL){
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);

      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
      msgbox.setInformativeText("recv NULL");
      msgbox.exec();
      return -1;
    }else if(recv_data[1] == (char)0x31){
      ui->widget->change_label_content(REFERENCE_BE_LOCATON);
    }else if(recv_data[1] == (char)0x33){
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(MACHINE_MALFUNCTION_TEXT);
      msgbox.exec();
      set_sliding_disabled(false,false);
      emit transmit_move_sliding(false,false);
      return -1;
    }else {
      QSettings communication_err_data("shanghaikairen","communication_error");
      communication_err_data.setValue("com_err_6",communication_err_data.value("com_errr_6").toInt() + 1);

      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText(QString(SLIDING_PLATE_NO_CHANGE_TEXT) + ":" + recv_data + "," + QString::number(measurement_flag));
      msgbox.exec();
      return -1;
    }
  set_sliding_disabled(true,false);
  emit transmit_move_sliding(true,false);
  if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) < 5){
      move_sliding_count++;
    }else{
      move_sliding_count = 0;
    }
  last_move_sliding_datetime = QDateTime::currentDateTime();
  return ALL_RIGHT;
}
void spectrummeasurement::set_sliding_disabled(bool out,bool in){
  ui->pushButton_5->setDisabled(in);
  ui->pushButton_4->setDisabled(out);
}

void spectrummeasurement::receive_transmit_count_time(int recv_data){
  emit transmit_count_time(recv_data);
}

void spectrummeasurement::stop_inspectoscope(){
  on_pushButton_2_clicked();
  countmeas->on_pushButton_2_clicked();
}

void spectrummeasurement::printer_result(){
  //QMessageBox::warning(this,"spectrum","printer!");
#if 1
  int enter = 0x0A;
  long long spectrum = 0xBFC1E2B2D7C6DCC4ll;//right
  long long reference = 0xf9d1bcbfceb2;
  long long wait_measurement = 0xF9D1A8B6EAB1;
  long long summitat = 0xDAD4BBCEE5B7ll;
  long long countdata_zhi = 0xB5D6FDCAC6BCll;
  int  sulphur = 0xF2C1;



  printer::transmit((void *)"========================================",SEGMENT_LENGTH);
  printer ::transmit(enter,1);

  //头：能谱测量，参考样
  printer::transmit(spectrum,8);
  printer ::transmit(enter,1);
  if(ui->widget->global_is_sample ==REFERENCE_BE_LOCATON)
    {
    printer::transmit(reference,6);
    }
  if(ui->widget->global_is_sample ==WAIT_BE_LOCATION)
    {
    printer::transmit(wait_measurement,6);
    }
  printer ::transmit(enter,1);

  //日期
  printer::printCurrentDateTime();

  //打印数据
  for (int column = 6; column >= 0; column -= 2) {
      for(int row=9;row >= 0;row--){
          QTableWidgetItem *item= ui->tableWidget->item(row,column);
          if(item == NULL){
              continue;
            }
          QString text = item->text();

          item= ui->tableWidget->item(row,column + 1);
          if(item == NULL){
              continue;
            }
          QString value = item->text();
          printer::transmit(text.toLocal8Bit().data(),text.size());
          printer::transmit((void *)"   ",3);
          printer::transmit(value.toLocal8Bit().data(),value.size());
          printer ::transmit(enter,1);
        }
    }
#endif
#if 0
  //打印坐标图
  //沿切纸方向打印 Y 轴
  char tmp_x_reverse = 0x00;
  int x_data_length = 2;
  int x_data_length_pre = 8;
  long long tmp_x_reverse1 = 0x284c1cll;

  long long tmp_x_reverse2_pre = 0x3333323231313030ll;
  long long tmp_x_reverse2 = 0x0a34ll;
  long long tmp_x_reverse3_pre = 0x3530353035303530ll;
  long long tmp_x_reverse3 = 0x0a30ll;
  long long tmp_x_reverse4_pre = 0x3030303030303030ll;
  long long tmp_x_reverse4 = 0x0a30ll;
  long long tmp_x_reverse5_pre = 0x3030303030303030ll;
  long long tmp_x_reverse5 = 0x0a30ll;

  printer::transmit(tmp_x_reverse1,3);

  printer::transmit(tmp_x_reverse2_pre,x_data_length_pre);
  printer::transmit(tmp_x_reverse2,x_data_length);

  printer::transmit(tmp_x_reverse3_pre,x_data_length_pre);
  printer::transmit(tmp_x_reverse3,x_data_length);

  printer::transmit(tmp_x_reverse4_pre,x_data_length_pre);
  printer::transmit(tmp_x_reverse4,x_data_length);

  printer::transmit(tmp_x_reverse5_pre,x_data_length_pre);
  printer::transmit(tmp_x_reverse5,x_data_length);
  printer::transmit(tmp_x_reverse);

  //沿走纸方向打印 X 轴及曲线
  //    0x1c 0x4d 50 0x02 0x35 0x30 0x0b 0x31 0x30 0x30 0x0b 0x00 0x01 0x80 0x00
  int trans_data_size = 1;
  int tmp_y_reverse_style = 0x05154d1c;//0x28==40
  int tmp_y_reverse1 = 0x31;
  int tmp_y_reverse2 = 0x32;
  int tmp_y_reverse3 = 0x33;
  int tmp_y_reverse4 = 0x34;
  int tmp_y_reverse5 = 0x35;

  char ll = 0x00;
  char print_line_data = 0x0b;
  int tmp_y_reverse_line1= 0x00ff01ll;//上面那两位决定离y轴的距离
  int tmp_y_reverse_line2= 0x000001ll;//上面那两位决定离y轴的距离

  printer::transmit(tmp_y_reverse_style,trans_data_size + 3);

  printer::transmit(tmp_y_reverse1,trans_data_size);
  printer::transmit(print_line_data);

  printer::transmit(tmp_y_reverse2,trans_data_size);
  printer::transmit(print_line_data);

  printer::transmit(tmp_y_reverse3,trans_data_size);
  printer::transmit(print_line_data);

  printer::transmit(tmp_y_reverse4,trans_data_size);
  printer::transmit(print_line_data);

  printer::transmit(tmp_y_reverse5,trans_data_size);
  printer::transmit(print_line_data);

  printer::transmit(ll);

  static int array_data[SPECTRUM_PAINTER_WIDTH] = {
    0,100,200,300,592,
    592,641,656,720,720,
    769,865,944,1089,1281,
    1360,1360,1473,1360,1296,
    1057,816,609,417,321,
    257,272,400,577,944,
    1498,2000,2576,2721,2192,
    1616,939,400,161,80,33,16,33,48,48,48,48};
  //  for(int column = 0;i <= 6;i += 2){
  //      for(int row = 0;i <10; i++){
  //          array_data[tmp_array_length++] = ui->tableWidget->item(row,column)->text().toInt();
  //        }
  //    }
  for (int i = 0; i <SPECTRUM_PAINTER_WIDTH; ++i) { //循环次数决定y轴的长度
      tmp_y_reverse_line1= 0;
      tmp_y_reverse_line1 = specture_array_data[i] / 25 * 2;//i /3 *i * i + 5
      tmp_y_reverse_line1 <<= 8;
      tmp_y_reverse_line1 |= (int)0x01;
      //qDebug()<< QString::number(tmp_y_reverse_line1,16);
      for (int i = 0; i <= 0; ++i) {
          printer::transmit(tmp_y_reverse_line2,3);
          printer::transmit(tmp_y_reverse_line1,3);
        }
    }

  //painter_histogram::spectrum_data = array_data;// 赋值#include "global.h"

  printer::transmit(ll);
  printer::transmit((void *)"   ",3);
  printer ::transmit(enter,1);
#endif

  //print  summit
  printer::transmit(sulphur,2);
  printer::transmit((void *)": ",2);
  printer::transmit(summitat,6);
  printer::transmit((void *)max_volumn.toLocal8Bit().data(),max_volumn.size());
  printer ::transmit(enter,1);
  printer::transmit((void *)"     ",4);
  printer::transmit(countdata_zhi,6);
  printer::transmit((void *)"     ",2);
  printer::transmit((void *)max_data.toLocal8Bit().data(),max_data.size());

  printer ::transmit(enter,1);
  printer::transmit((void *)"   ",3);
  printer::transmit((void *)"========================================",SEGMENT_LENGTH);
  printer ::transmit(enter,1);
  printer::transmit((void *)"   ",3);
  printer ::transmit(enter,1);
}

#if 1
void spectrummeasurement::start_steady_summit(){
  on_pushButton_clicked();
  measurement_flag = MEASUREMENT_STEADY_SUMMIT;
}
#endif

void spectrummeasurement::on_pushButton_6_clicked()
{
  for(int i = 0; i < SPECTRUM_PAINTER_WIDTH;i++){
      painter_histogram::spectrum_data[i] = specture_array_data[i];
    }
  painter_result->spectrum_show_and_update(summit_value);
}
