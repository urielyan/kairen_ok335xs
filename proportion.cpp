
#include "proportion.h"
#include "ui_proportion.h"
#include "global.h"
#include "datasave.h"

#include <QMessageBox>

proportion::proportion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::proportion)
{
    p_mySettings = MeasurementDataSave::instance();

    ui->setupUi(this);

    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for(int i=0;i<allPButtons.count();i++){
        allPButtons[i]->setFocusPolicy(Qt::NoFocus);//设置所有按钮没有焦点
        QString tmpstr=allPButtons[i]->objectName();
        //  tmpstr=tmpstr.remove("b_");
        if(tmpstr.length()==3){
            QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slot_keyNumPressed()));
        }
    }

    ui->lineEdit->setText(p_mySettings->value(MYSETTINGS_PROPORTION(1)).toString());
    ui->lineEdit_2->setText(p_mySettings->value(MYSETTINGS_PROPORTION(2)).toString());

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
    ui->lineEdit->setText(p_mySettings->value(MYSETTINGS_PROPORTION(1)).toString());
    ui->lineEdit_2->setText(p_mySettings->value(MYSETTINGS_PROPORTION(2)).toString());
    this->close();
}

void proportion::on_b_sure_clicked()
{
    if(ui->lineEdit->text().size() < 1 || ui->lineEdit_2->text().size() <1){
        WinInforListDialog::instance()->showMsg(tr("您没有输入值，请重新输入"));
    }
    p_mySettings->setValue(MYSETTINGS_PROPORTION(1),ui->lineEdit->text());
    p_mySettings->setValue(MYSETTINGS_PROPORTION(2),ui->lineEdit_2->text());

    WinInforListDialog::instance()->showMsg(tr("自动系数已存储"));
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
