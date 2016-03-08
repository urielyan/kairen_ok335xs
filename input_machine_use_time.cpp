#include "input_machine_use_time.h"
#include "ui_input_machine_use_time.h"
#include "global.h"

//extern QSettings mysettings;

input_machine_use_time::input_machine_use_time(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::input_machine_use_time)
{
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
}

input_machine_use_time::~input_machine_use_time()
{
  delete ui;
}
void input_machine_use_time::slot_keyNumPressed(){
  QString tmpstr=sender()->objectName();
  tmpstr.remove("b_");

  if(ui->lineEdit->text().size() > 6){
      return;
    }

  ui->lineEdit->setText(ui->lineEdit->text() + tmpstr);
}

#define DECODE 1
#define PRODUCT_SERIAL 2
static int flag = 0;

void input_machine_use_time::input_decode_serial(){
	//当产品的使用时间到达最大使用时间时，则进入此函数。需要输入密码才能继续使用此仪器
  if(flag == DECODE)return;
  ui->l_title->setText("输入解密密码");
  ui->l_notation->setText("请输入解密密码");
  ui->lineEdit->setText("");
  ui->b_return->setText("退格");
  ui->textBrowser->show();
  //ui->b_return->setDisabled(true);
  flag = DECODE;
  this->showFullScreen();
}

void input_machine_use_time::input_product_serial(){
	//在管理员菜单下的输入产品编号和使用日期
  ui->l_title->setText("输入使用时间");
  ui->l_notation->setText("请输入使用时间");
  ui->lineEdit->setText("");
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
  if(line_str.size() < 7){
      return;
    }
  if(flag == DECODE){
      if(line_str.toInt() == 1111111){
          mysettings.setValue("most_use_time",0);
          mysettings.setValue("machine_used_time",0);
          this->close();
        }
    }else if(flag == PRODUCT_SERIAL){
      QString product = line_str.left(6);
      QString time = line_str.right(1);
      mysettings.setValue("most_use_time",time.toInt());
      mysettings.setValue("product_serial",product);
    }
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
