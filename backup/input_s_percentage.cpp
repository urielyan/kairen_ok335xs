#include "input_s_percentage.h"
#include "ui_input_s_percentage.h"
#include "global.h"
#include "wininforlistdialog.h"
#include "datasave.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDebug>

int WinInputSPercentage::s_calibrateNumber = 50;

WinInputSPercentage::WinInputSPercentage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::input_S_percentage)
{
  p_mySettings = MeasurementDataSave::instance();
  ui->setupUi(this);
  ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));

  initTableWidget();

  initSignalSlotConnect();
  ui->label->setObjectName("title");
  }

WinInputSPercentage::~WinInputSPercentage()
{
  delete ui;
}

void WinInputSPercentage::clear_all_tablewidget()
{
  for(int tmpNumber = 0; tmpNumber < s_calibrateNumber ;tmpNumber++){
      p_mySettings->setValue(MYSETTINGS_CALIBRATE_S_INPUT(tmpNumber), "0.0000");
      p_mySettings->setValue(MYSETTINGS_CALIBRATE_S_DATA(tmpNumber), "");
    }

  //TODO: down 4 line: useless
  for(int row = 0;row < s_calibrateNumber;row++)
    {
      ui->tableWidget->item(row,0)->setText("0.0000");
    }
  p_mySettings->setValue(MYSETTINGS_CALIBRATE_COUNT, 0);
}

void WinInputSPercentage::showAndUpdateData()
{
  initTableWidgetData();

  this->showFullScreen();
}

void WinInputSPercentage::slot_keyNumPressed(){
  //判断是否是第0列，若not则返回
  if(ui->tableWidget->currentItem() == NULL || ui->tableWidget->currentItem()->column() != 0 )
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

void WinInputSPercentage::on_b_abandon_clicked()
{
  //don't save button
  initTableWidgetData();
  this->close();
}

void WinInputSPercentage::on_b_clear_clicked()
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

void WinInputSPercentage::on_b_backspace_clicked()
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

void WinInputSPercentage::slotPointClicked()
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

void WinInputSPercentage::on_b_sure_clicked()
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
      for(row = 0 ; row < s_calibrateNumber ; row++){
          QString calibrateInputKey = MYSETTINGS_CALIBRATE_S_INPUT(row);
          p_mySettings->setValue(calibrateInputKey,  WinInforListDialog::instance()->doubleToCompleteDouble(ui->tableWidget->item(row,0)->text().toDouble()));
        }
      WinInforListDialog::instance()->showMsg(tr("您已保存了您输入的数据，现在可以计算kb值了"));
      this->close();
    }else{
      this->close();
  }
}

void WinInputSPercentage::initTableWidget()
{
  //set tableWidget size
  ui->tableWidget->setRowCount(s_calibrateNumber);
  ui->tableWidget->setColumnCount(3);

  for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
  {
      ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i + 1) + "#"));
      for (int j = 0; j < ui->tableWidget->columnCount(); ++j)
      {
          ui->tableWidget->setItem(i, j, new QTableWidgetItem(""));
      }
  }

  //set tableWidget column header and column width.
  for(int columnCount = 0; columnCount < 3; columnCount++)
    {
      ui->tableWidget->horizontalHeaderItem(columnCount)->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->tableWidget->horizontalHeaderItem(0)->setText(tr("硫含量"));
  ui->tableWidget->horizontalHeaderItem(1)->setText(tr("标定样计数值"));
  ui->tableWidget->horizontalHeaderItem(2)->setText(tr("参考样计数值"));
  ui->tableWidget->setColumnWidth(0, DESKTOP_WIDTH/6 - 15);
  ui->tableWidget->setColumnWidth(1, DESKTOP_WIDTH/5 - 5);
  ui->tableWidget->setColumnWidth(2, DESKTOP_WIDTH/5 - 5);

  initTableWidgetData();

  ui->tableWidget->setCurrentCell(0,0);
}

void WinInputSPercentage::initTableWidgetData()
{
  //set tableWidget row header ,row height, item isEnabled, item text.
  ui->tableWidget->clearContents();
  for(int i = 0; i < s_calibrateNumber; i++)
    {
      //set 0 column text.
      ui->tableWidget->setItem(i, 0,new QTableWidgetItem(
                                 p_mySettings->value(MYSETTINGS_CALIBRATE_S_INPUT(i)).toString() ));

      //set 1,2 column object and text;
      QStringList stringCalibrateData = p_mySettings->value(MYSETTINGS_CALIBRATE_S_DATA(i)).toString().split("/");
      if(2 == stringCalibrateData.size())
        {
          ui->tableWidget->setItem(i, 1,new QTableWidgetItem(stringCalibrateData.at(0)));
          ui->tableWidget->setItem(i, 2, new QTableWidgetItem(stringCalibrateData.at((1))));
        }else
        {
          ui->tableWidget->setItem(i, 1,new QTableWidgetItem(""));
          ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
        }

      //set column 1,2 disSelectAbled.
      ui->tableWidget->item(i, 1)->setFlags( ui->tableWidget->item(i, 1)->flags() & (~Qt::ItemIsSelectable) );
      ui->tableWidget->item(i, 2)->setFlags( ui->tableWidget->item(i, 2)->flags() & (~Qt::ItemIsSelectable) );

      //set all item align center;
      for(int columnCount = 0; columnCount < 3; columnCount++)
        {
          ui->tableWidget->item(i, columnCount)->setTextAlignment(Qt::AlignCenter);
        }

      //set all row height
      ui->tableWidget->setRowHeight(i, DESKTOP_HEIGHT/17);
    }
}

void WinInputSPercentage::initSignalSlotConnect()
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
