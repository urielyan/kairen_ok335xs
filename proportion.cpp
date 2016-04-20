
#include "proportion.h"
#include "ui_proportion.h"
#include "global.h"

#include <QMessageBox>

//extern QSettings mysettings;

proportion::proportion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::proportion)
{
    ui->setupUi(this);

    // not set default value
//    if(!mysettings.contains("proportion_1"))
//      {
//        mysettings.setValue("proportion_1", 0.1);
//        mysettings.setValue("proportion_2", 1);
//      }

    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for(int i=0;i<allPButtons.count();i++){
        allPButtons[i]->setFocusPolicy(Qt::NoFocus);//设置所有按钮没有焦点
        QString tmpstr=allPButtons[i]->objectName();
        //  tmpstr=tmpstr.remove("b_");
        if(tmpstr.length()==3){
            QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keyNumPressed()));
        }
    }

    ui->lineEdit->setText(mysettings.value("proportion_1").toString());
    ui->lineEdit_2->setText(mysettings.value("proportion_2").toString());

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
}

void proportion::showAndInit()
{
  this->showFullScreen();
}

proportion::~proportion()
{
    delete ui;
}

void proportion::slot_keyNumPressed(){
    QString tmpstr=sender()->objectName();
    tmpstr.remove("b_");
    QString current_text;
    if(ui->lineEdit->hasFocus()){
        current_text = ui->lineEdit->text();
        current_text += tmpstr;
        if(current_text.size() > 6) return;
        ui->lineEdit->setText(current_text);
    }else if (ui->lineEdit_2->hasFocus()){
        current_text = ui->lineEdit_2->text();
        current_text += tmpstr;
        if(current_text.size() > 6) return;
        ui->lineEdit_2->setText(current_text);
    }
}

void proportion::on_pushButton_3_clicked()
{
    ui->lineEdit->setText(mysettings.value("proportion_1").toString());
    ui->lineEdit_2->setText(mysettings.value("proportion_2").toString());
    this->close();
}

void proportion::on_b_sure_clicked()
{
    if(ui->lineEdit->text().size() < 1 || ui->lineEdit_2->text().size() <1){
        QMessageBox msgbox;
        msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
        msgbox.setText("您没有输入值，请重新输入");
        msgbox.exec();
    }
    mysettings.setValue("proportion_1",ui->lineEdit->text());
    mysettings.setValue("proportion_2",ui->lineEdit_2->text());
    QMessageBox msgbox2;
    msgbox2.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    msgbox2.setText("自动系数已存储");
    msgbox2.exec();
    this->on_pushButton_3_clicked();
}

void proportion::on_b_backspace_clicked()
{
    QString current_text;
    if(ui->lineEdit->hasFocus()){
        current_text = ui->lineEdit->text();
        current_text.chop(1);
        ui->lineEdit->setText(current_text);
    }else if (ui->lineEdit_2->hasFocus()){
        current_text = ui->lineEdit_2->text();
        current_text.chop(1);
        ui->lineEdit_2->setText(current_text);
    }
}

void proportion::on_b_clear_clicked()
{
    if(ui->lineEdit->hasFocus()){
        ui->lineEdit->setText("");
    }else if (ui->lineEdit_2->hasFocus()){
        ui->lineEdit_2->setText("");
    }
}

void proportion::on_b_point_clicked()
{
    QString current_text;
    if(ui->lineEdit->hasFocus()){
        if(ui->lineEdit->text().size() == 0) return;
        if(ui->lineEdit->text().contains("."))return;
        ui->lineEdit->setText(ui->lineEdit->text().append("."));
    }else if (ui->lineEdit_2->hasFocus()){
        if(ui->lineEdit_2->text().size() == 0) return;
        if(ui->lineEdit_2->text().contains("."))return;
        ui->lineEdit_2->setText(ui->lineEdit_2->text().append("."));
    }
}
