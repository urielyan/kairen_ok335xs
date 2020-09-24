#include "input_machine_use_time.h"
#include "ui_input_machine_use_time.h"
#include "global.h"
#include "datasave.h"
#include "wininforlistdialog.h"

input_machine_use_time::input_machine_use_time(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::input_machine_use_time)
{
    p_mySettings = MeasurementDataSave::instance();

    //this->setWindowFlags(Qt::Widget | Qt::WindowStaysOnTopHint);

    qDebug() << "_________TEST_______";
//    qDebug() << seirialSumLastNumber(20201235l);
//    qDebug() << seirialSumLastNumber(20200005l);

//    qDebug() << serialAdd67896789("10000000");
//    qDebug() << serialAdd67896789("16000000");
//    qDebug() << serialAdd67896789("16000009");

    ui->setupUi(this);

    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for(int i=0;i<allPButtons.count();i++){
        QString tmpstr=allPButtons[i]->objectName();
        //  tmpstr=tmpstr.remove("b_");
        if(tmpstr.length()==3){
            QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keyNumPressed()));
        }
    }

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
    ui->l_title->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));

//    ui->backspaceButton->hide();
//    ui->clearButton->hide();
}

input_machine_use_time::~input_machine_use_time()
{
  delete ui;
}
void input_machine_use_time::slot_keyNumPressed(){
  QString tmpstr=sender()->objectName();
  tmpstr.remove("b_");

  if(ui->lineEdit->text().size() > 10){
      return;
    }

  ui->lineEdit->setText(ui->lineEdit->text() + tmpstr);
}

int input_machine_use_time::seirialSumLastNumber(long code)
{
    int sum = 0;
    while (code > 0) {
        int lastNumber = code % 10;
        code = code / 10;
        sum += lastNumber;
    }

    return QString::number(sum).right(1).toInt();
}

QString input_machine_use_time::serialAdd67896789(QString serial)
{
    QString dest("00000000");
    QString fixStr("67896789");
    if (serial.size() != fixStr.size())
    {
        return dest;
    }


    dest = "";
    for (int i = 0; i < fixStr.size(); ++i)
    {
        int fixSignalChar = fixStr.mid(i, 1).toInt();
        int serialSignalCHar = serial.mid(i, 1).toInt();
        dest.append(QString::number(fixSignalChar + serialSignalCHar).right(1));
    }

    return dest;
}

#define DECODE 1
#define PRODUCT_SERIAL 2
static int flag = 0;

void input_machine_use_time::input_decode_serial(){
	//当产品的使用时间到达最大使用时间时，则进入此函数。需要输入密码才能继续使用此仪器
    if(flag == DECODE)
//    {
//        this->showFullScreen();
//        return;
//    }
  ui->l_title->setText("输入解密密码");
  ui->l_notation->setText("请输入解密密码");
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("解码序列号为10位");
  ui->b_return->setText("退格");
  ui->textBrowser->show();
  ui->serialLabel->setText(p_mySettings->value(MYSETTINGS_SERIAL).toString());
  //ui->b_return->setDisabled(true);
  flag = DECODE;
  this->showFullScreen();
}

void input_machine_use_time::input_product_serial(){

    if (p_mySettings->value(MYSETTINGS_SERIAL).toString().size() == 8
            )
    {
        WinInforListDialog::instance()->showMsg(
                    "您已输入过序列号!" + p_mySettings->value(MYSETTINGS_SERIAL).toString()
                    );
        return;
    }

	//在管理员菜单下的输入产品编号和使用日期
  ui->l_title->setText("请输入序列号");
  //ui->l_notation->setText("请输入使用时间");
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("序列号为9位");
  ui->b_return->setText("返回");
  ui->textBrowser->hide();
  //ui->b_return->setDisabled(false);
  flag = PRODUCT_SERIAL;
  this->showFullScreen();
}

void input_machine_use_time::on_b_ok_clicked()
{
	//有两个功能：解密，输入产品编号和使用日期。
  QString line_str = ui->lineEdit->text();

  if(flag == DECODE){
      if(line_str.size() != 10){
          //WinInforListDialog::instance()->showMsg(tr("解码序列号应该为10位！"));
          ui->l_notation->setText("解码序列号应该为10位, 请重新输入！");
                  return;
      }
      QString serial = line_str.left(8);
      int code = line_str.mid(8, 1).toInt();
      int lastNUmber = line_str.right(1).toInt();

      QString localMachineSerial = p_mySettings->value(MYSETTINGS_SERIAL).toString();
      localMachineSerial = serialAdd67896789(localMachineSerial);

      if ((serial != localMachineSerial)
              )
      {
        //WinInforListDialog::instance()->showMsg(tr("输入序列号错误!"));
          ui->l_notation->setText("输入序列号错误, 请重新输入！");
        return;
      }
      if (code != 1 && code != 2)
      {
        //WinInforListDialog::instance()->showMsg(tr("输入序列号错误!"));
          ui->l_notation->setText("输入序列号错误, 请重新输入！");
        return;
      }


      if (code == 1)
      {
          // complete decode 完全解密
          if (seirialSumLastNumber(localMachineSerial.toLong()) == lastNUmber)
          {
              //如果序列号和数据保持一致，并且最后一位校验也成功，则完全解密
              p_mySettings->setValue(MYSETTINGS_MAXIMAL_USE_TIME,0);
              p_mySettings->setValue(MYSETTINGS_SERIAL, 0);
              p_mySettings->setValue(MYSETTINGS_USED_TIME,0);
              flag = 0;
              this->close();
              return;
          }
          else{
            //WinInforListDialog::instance()->showMsg(tr("输入序列号错误!"));
            ui->l_notation->setText("输入序列号错误, 请重新输入！");
            return;
          }
      }else if (code == 2)
      {
          //go on lock
          p_mySettings->setValue(MYSETTINGS_USED_TIME,0);
          p_mySettings->setValue(MYSETTINGS_SERIAL, QString::number(qrand()).right(8));
          p_mySettings->setValue(MYSETTINGS_MAXIMAL_USE_TIME,lastNUmber);
            flag = 0;
            this->close();
            return;
      }


    }else if(flag == PRODUCT_SERIAL){
      if(line_str.size() != 9){
          //WinInforListDialog::instance()->showMsg(tr("序列号应该为9位！"));
           ui->l_notation->setText("序列号应该为9位, 请重新输入！");
          return;
        }
      QString product = line_str.left(8);
      QString time = line_str.right(1);
      p_mySettings->setValue(MYSETTINGS_MAXIMAL_USE_TIME,time.toInt());
      p_mySettings->setValue(MYSETTINGS_SERIAL,product);
      p_mySettings->setValue(MYSETTINGS_USED_TIME,0);
      on_b_return_clicked();
    }

      ui->lineEdit->setText("");
      this->close();
      flag = 0;
}

void input_machine_use_time::on_b_return_clicked()
{
	//有两个功能：解密，输入产品编号和使用日期。
  if(flag == DECODE){
      QString line_string = ui->lineEdit->text();
      line_string.chop(1);
      ui->lineEdit->setText(line_string);
    }else if(flag == PRODUCT_SERIAL ){
      ui->lineEdit->setText("");
      this->close();
    }
}

void input_machine_use_time::on_clearButton_clicked()
{
    ui->lineEdit->setText("");
}

void input_machine_use_time::on_backspaceButton_clicked()
{
    QString lineEditStr = ui->lineEdit->text();
    lineEditStr.chop(1);
    ui->lineEdit->setText(lineEditStr);
}
