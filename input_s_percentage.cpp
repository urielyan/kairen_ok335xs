#include "input_s_percentage.h"
#include "ui_input_s_percentage.h"
#include "global.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>

//extern QSettings mysettings;

WidgetInputSPercentage::WidgetInputSPercentage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::input_S_percentage)
{
  ui->setupUi(this);

  int tmpnumber;
  QString tmpstr = "calibrate_input_s_";

  //mysettings.clear();
  /*
        If mysettings have not calibrate_input_s_ + i ,
        build them and set them "0.0000".
    */
  if(!mysettings.contains("calibrate_input_s_1")){
      for(tmpnumber = 1; tmpnumber <= 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          qDebug() << tmpstr;
          mysettings.setValue(tmpstr,"0.0000");
          tmpstr = "calibrate_input_s_";
        }
    }

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));

  initTableWidget();

  initSignalSlotConnect();
  }

WidgetInputSPercentage::~WidgetInputSPercentage()
{
  delete ui;
}

void WidgetInputSPercentage::clear_all_tablewidget(){
  /*
        clear tablewidget and mysettings.value("calibrate_input_s_ + i");
        It's used to clear data in calibrate widget.

        clear calibratemeasurement_count settings;
    */
  int tmpnumber,row,column;
  QString tmpstr = "calibrate_input_s_";
  for(tmpnumber = 1; tmpnumber <= 12 ;tmpnumber++){
      tmpstr.append(QString("%1").arg(tmpnumber));
      mysettings.setValue(tmpstr,"0.0000");
      tmpstr = "calibrate_input_s_";
    }

  for(row = 0;row < 12;row++)
    {
        ui->tableWidget->item(row,0)->setText("0.0000");
      }
  mysettings.setValue("calibratemeasurement_count",0);
}

void WidgetInputSPercentage::slot_keyNumPressed(){
  //判断是否是第0列，若not则返回
  if(ui->tableWidget->currentItem()->column() != 0 )
    {
      return;
    }

  QString tmpstr=sender()->objectName();
  tmpstr.remove("b_");

  QTableWidgetItem *tmpitem = ui->tableWidget->currentItem();
  if(tmpitem == NULL ||
     ui->tableWidget->currentColumn() != 0){
      return;
    }

  //when text contains "." ,judge whether need continue Changing item text.
  if(   !ui->tableWidget->currentItem()->text().contains(".") &&
        ui->tableWidget->currentItem()->text() != NULL &&
        ui->tableWidget->currentItem()->text().toDouble() == 0)
    return;
  QString itemtext = tmpitem->text();
  if(!itemtext.compare("0.0000")){itemtext.clear();}
  itemtext += tmpstr;

  int tmp_Row = ui->tableWidget->currentRow();
  int tmp_Column = ui->tableWidget->currentColumn();
  if(itemtext.size() == 6)
    {
      tmp_Row++;
    }else if(itemtext.size() > 6)
    {
      tmp_Row++;
      itemtext.chop(1);
    }
  ui->tableWidget->currentItem()->setText(itemtext);
  ui->tableWidget->setCurrentCell(tmp_Row,tmp_Column);
}

void WidgetInputSPercentage::on_b_abandon_clicked()
{
  //don't save button
  this->close();
}

void WidgetInputSPercentage::on_b_clear_clicked()
{
  //clear button
  //判断是否是第0列，若not则返回
  if(ui->tableWidget->currentItem()->column() != 0 )
    {
      return;
    }
  QTableWidgetItem *tmpitem;
  if((tmpitem = ui->tableWidget->currentItem()) == NULL){
      return;
    }
  ui->tableWidget->currentItem()->setText("");

  ui->tableWidget->setCurrentItem(tmpitem);
}

void WidgetInputSPercentage::on_b_backspace_clicked()
{
  //backspace button

  //判断是否是第0列，若not则返回
  if(ui->tableWidget->currentItem()->column() != 0 )
    {
      return;
    }

  QTableWidgetItem *tmpitem;
  if((tmpitem = ui->tableWidget->currentItem()) == NULL){
      return;
    }
  QString itemtext = tmpitem->text();
  itemtext.chop(1);
  ui->tableWidget->currentItem()->setText(itemtext);

  ui->tableWidget->setCurrentItem(tmpitem);
}

void WidgetInputSPercentage::slotPointClicked()
{
  //point button

  //判断是否是第0列，若not则返回
  if(ui->tableWidget->currentItem()->column() != 0 )
    {
      return;
    }

  QTableWidgetItem *tmpitem;
  if((tmpitem = ui->tableWidget->currentItem()) == NULL){
      return;
    }
  QString itemtext = tmpitem->text();
  if (itemtext.size() == 0 ||
      itemtext.size() > 4 ||
      itemtext.contains("."))
    {
      return;
    }
  itemtext += ".";
  ui->tableWidget->currentItem()->setText(itemtext);

  ui->tableWidget->setCurrentItem(tmpitem);
}

void WidgetInputSPercentage::on_b_sure_clicked()
{
  //to save button
  int row;
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText(tr("你要保存你输入的数据么!"));
  msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
  msgbox.setDefaultButton(QMessageBox::Save);
  int ret = msgbox.exec();
  if (QMessageBox::Save == ret){
      for(row = 0 ; row < 12 ; row++){
          QString tmpsettingsstr = QString("calibrate_input_s_%1").arg(row);
          mysettings.setValue(tmpsettingsstr,ui->tableWidget->item(row,0)->text());
        }
      QMessageBox box;
      box.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      box.setText(tr("您已保存了您输入的数据，现在可以计算kb值了!"));
      box.exec();
      this->close();
    }else{
      this->close();
  }
}

void WidgetInputSPercentage::initTableWidget()
{
  //set tableWidget size
  ui->tableWidget->setRowCount(12);
  ui->tableWidget->setColumnCount(3);

  //set tableWidget column header and column width.
  for(int columnCount = 0; columnCount < 3; columnCount++)
    {
      ui->tableWidget->horizontalHeaderItem(columnCount)->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->tableWidget->horizontalHeaderItem(0)->setText(tr("硫含量"));
  ui->tableWidget->horizontalHeaderItem(1)->setText(tr("标定样计数值"));
  ui->tableWidget->horizontalHeaderItem(2)->setText(tr("参考样计数值"));
  ui->tableWidget->setColumnWidth(0, DESKTOP_WIDTH/6);
  ui->tableWidget->setColumnWidth(1, DESKTOP_WIDTH/5);
  ui->tableWidget->setColumnWidth(2, DESKTOP_WIDTH/5);

  //set tableWidget row header ,row height, item isEnabled, item text.
  for(int i = 0; i < 12; i++)
    {
      //set vertical header.
      ui->tableWidget->verticalHeaderItem(i)->setText(QString::number(i + 1) + "#");

      //set 0 column text.
      ui->tableWidget->setItem(i, 0,new QTableWidgetItem(
                                 mysettings.value(QString("calibrate_input_s_") + QString::number(i + 1)).toString()   ));

      //set 1,2 column object and text;
      QStringList stringCalibrateData = mysettings.value(QString("s_count_data_") + QString::number(i + 1)).toString().split("/");
      if(2 == stringCalibrateData.size())
        {
          ui->tableWidget->setItem(i, 1,new QTableWidgetItem(stringCalibrateData.at(0)));
          ui->tableWidget->setItem(i, 2, new QTableWidgetItem(stringCalibrateData.at((1))));
        }else
        {
          ui->tableWidget->setItem(i, 1,new QTableWidgetItem(""));
          ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
        }

      //set column 1,2 disabled.
      ui->tableWidget->item(i, 1)->setFlags( ui->tableWidget->item(i, 1)->flags() & (~Qt::ItemIsEnabled) );
      ui->tableWidget->item(i, 2)->setFlags( ui->tableWidget->item(i, 2)->flags() & (~Qt::ItemIsEnabled) );

      //set all item align center;
      for(int columnCount = 0; columnCount < 3; columnCount++)
        {
          ui->tableWidget->item(i, columnCount)->setTextAlignment(Qt::AlignCenter);
        }

      //set all row height
      ui->tableWidget->setRowHeight(i, DESKTOP_HEIGHT/17);
    }
  ui->tableWidget->setCurrentCell(0,0);
}

void WidgetInputSPercentage::initSignalSlotConnect()
{
  //point signal and slot.
  connect(ui->b_point, SIGNAL(clicked()), this, SLOT(slotPointClicked()));

  //define the number key slots.
  QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
  for(int i=0;i<allPButtons.count();i++){
      QString tmpstr=allPButtons[i]->objectName();
      //  tmpstr=tmpstr.remove("b_");
      if(tmpstr.length()==3){
          QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keyNumPressed()));
        }
    }
}
