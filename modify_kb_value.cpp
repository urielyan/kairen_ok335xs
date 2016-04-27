#include "modify_kb_value.h"
#include "ui_modify_kb_value.h"
#include "global.h"

#include <QMessageBox>

//extern  QSettings mysettings;


modify_kb_value::modify_kb_value(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::modify_kb_value)
{
  ui->setupUi(this);

  //qDebug() <<;
  //define the number key slots.
  ui->label_a2->setText("");
  ui->lineEdit_a2->hide();
  ui->lineEdit_ka0->setFocus();

  QStringList tmplist = mysettings.value(QString("work_curve_1"))\
      .toString().split(";");
  if(3 == tmplist.size()){
      if(tmplist[0].split("=").size() == 2){
          ui->lineEdit_ba1->setText(tmplist[1].split("=")[1]);
          ui->lineEdit_ka0->setText(tmplist[0].split("=")[1]);
        }
    }
  QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
  for(int i=0;i<allPButtons.count();i++){
      QString tmpstr=allPButtons[i]->objectName();
      allPButtons[i]->setFocusPolicy(Qt::NoFocus);
      //  tmpstr=tmpstr.remove("b_");
      if(tmpstr.length()==3){
          QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keynumberPressed()));
        }
    }

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
  ui->comboBox->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
}

modify_kb_value::~modify_kb_value()
{
  delete ui;
}

void modify_kb_value::slot_keynumberPressed(){
  QString tmpstr=sender()->objectName();
  tmpstr.remove("b_");
  if(ui->lineEdit_ka0->hasFocus()){
      if(ui->lineEdit_ka0->text().size() > 7) return;
      ui->lineEdit_ka0->setText(ui->lineEdit_ka0->text().append(tmpstr));
    }else if(ui->lineEdit_ba1->hasFocus()){
      if(ui->lineEdit_ba1->text().size() > 7) return;
      ui->lineEdit_ba1->setText(ui->lineEdit_ba1->text().append(tmpstr));
    }else if(ui->lineEdit_a2->hasFocus()){
      if(ui->lineEdit_a2->text().size() > 7) return;
      ui->lineEdit_a2->setText(ui->lineEdit_a2->text().append(tmpstr));
    }
}

void modify_kb_value::on_b_abandon_clicked()
{
  ui->lineEdit_ka0->setFocus();
  ui->comboBox->setCurrentIndex(1);
  ui->comboBox->setCurrentIndex(0);
  this->close();
}

void modify_kb_value::on_comboBox_currentIndexChanged(const QString &arg1)
{
  ui->lineEdit_a2->setText("");
  ui->lineEdit_ba1->setText("");
  ui->lineEdit_ka0->setText("");

  if(arg1.toInt() <=5){
      ui->lineEdit_a2->hide();
      ui->label_a2->setText("");
      ui->label_b_a1->setText("b=");
      ui->label_k_a0->setText("k=");

      QStringList tmplist = mysettings.value(QString("work_curve_%1").arg(arg1.toInt()))\
          .toString().split(";");
      if(3 != tmplist.size())return;
      if(tmplist[0].split("=").size() == 2){

          ui->lineEdit_ba1->setText(tmplist[1].split("=")[1]);
          ui->lineEdit_ka0->setText(tmplist[0].split("=")[1]);
          //ui->lineEdit_a2->setText(tmplist[2].split("=")[1]);
        }
    }else{
      ui->lineEdit_a2->show();
      ui->label_k_a0->setText("a0=");
      ui->label_b_a1->setText("a1=");
      ui->label_a2->setText("a2=");

      QStringList tmplist = mysettings.value(QString("work_curve_%1")\
                                             .arg(ui->comboBox->currentText().toInt())).toString().split(";");
      if(3 != tmplist.size())return;
      if(tmplist[0].split("=").size() == 2){
          ui->lineEdit_ba1->setText(tmplist[1].split("=")[1]);
          ui->lineEdit_ka0->setText(tmplist[0].split("=")[1]);
          ui->lineEdit_a2->setText(tmplist[2].split("=")[1]);
        }
    }
  ui->lineEdit_ka0->setFocus();
}
void modify_kb_value::on_b_point_clicked()
{
  if(ui->lineEdit_ka0->hasFocus()){
      if(ui->lineEdit_ka0->text() == "-" || ui->lineEdit_ka0->text().size() == 0) return;
      if(ui->lineEdit_ka0->text().contains("."))return;
      ui->lineEdit_ka0->setText(ui->lineEdit_ka0->text().append("."));
    }else if(ui->lineEdit_ba1->hasFocus()){
      if(ui->lineEdit_ba1->text() == "-" || ui->lineEdit_ba1->text().size() == 0)return;
      if(ui->lineEdit_ba1->text().contains("."))return;
      ui->lineEdit_ba1->setText(ui->lineEdit_ba1->text().append("."));
    }else if(ui->lineEdit_a2->hasFocus()){
      if(ui->lineEdit_a2->text() == "-" || ui->lineEdit_a2->text().size() == 0)return;
      if(ui->lineEdit_a2->text().contains("."))return;
      ui->lineEdit_a2->setText(ui->lineEdit_a2->text().append("."));
    }
}

void modify_kb_value::on_b_negative_clicked()
{
  if(ui->lineEdit_ka0->hasFocus()){
      if(ui->lineEdit_ka0->text().size() != 0)return;
      ui->lineEdit_ka0->setText("-");
    }else if(ui->lineEdit_ba1->hasFocus()){
      if(ui->lineEdit_ba1->text().size() != 0)return;
      ui->lineEdit_ba1->setText("-");
    }else if(ui->lineEdit_a2->hasFocus()){
      if(ui->lineEdit_a2->text().size() != 0)return;
      ui->lineEdit_a2->setText("-");
    }
}

void modify_kb_value::on_b_backspace_clicked()
{
  if(ui->lineEdit_ka0->hasFocus()){
      if(ui->lineEdit_ka0->text().size() == 0)return;
      QString tmp = ui->lineEdit_ka0->text();
      tmp.chop(1);
      ui->lineEdit_ka0->setText(tmp);
    }else if(ui->lineEdit_ba1->hasFocus()){
      if(ui->lineEdit_ba1->text().size() == 0)return;
      QString tmp = ui->lineEdit_ba1->text();
      tmp.chop(1);
      ui->lineEdit_ba1->setText(tmp);
    }else if(ui->lineEdit_a2->hasFocus()){
      if(ui->lineEdit_a2->text().size() == 0)return;
      QString tmp = ui->lineEdit_a2->text();
      tmp.chop(1);
      ui->lineEdit_a2->setText(tmp);
    }
}

void modify_kb_value::on_b_sure_clicked()
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText("您要保存您输入的数据么?");
  msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
  int ret = msgbox.exec();
  if(QMessageBox::Save == ret){
      if((ui->lineEdit_ka0->text().size() == 0)  || (ui->lineEdit_ba1->text().size() == 0)){
          QMessageBox msgboxerr;
          msgboxerr.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          msgboxerr.setText("您输入的数据不完整，请重新输入");
          msgboxerr.exec();
          return;
        }
      QString tmpstr = "k=" + ui->lineEdit_ka0->text() + ";" + "b=" + ui->lineEdit_ba1->text() + ";";
      if (ui->comboBox->currentIndex() >= 5){
          if(ui->lineEdit_a2->text().size() == 0){
              QMessageBox msgboxerr;
              msgboxerr.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
              msgboxerr.setText("您输入的数据不完整，请重新输入");
              msgboxerr.exec();
              return;
            }
          tmpstr += "r=" + ui->lineEdit_a2->text();
        }
      mysettings.setValue(QString("work_curve_%1").arg(ui->comboBox->currentText().toInt()),tmpstr);
      mysettings.setValue(QString("real_compute_kbr_%1").arg(ui->comboBox->currentText().toInt()),tmpstr);
      QMessageBox msgbox2;
      msgbox2.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox2.setText("您输入的数据已保存！");
      msgbox2.exec();
      on_b_abandon_clicked();
      //this->close();
    }
}

void modify_kb_value::show_and_refresh(){
  on_comboBox_currentIndexChanged(QString("1"));
  this->showFullScreen();
}
