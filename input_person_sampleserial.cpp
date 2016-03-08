#include <QDebug>

#include "input_person_sampleserial.h"
#include "ui_input_person_sampleserial.h"
#include "global.h"

#include <QString>

input_person_sampleSerial::input_person_sampleSerial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input_person_sampleSerial)
{
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

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));

}
QString input_person_sampleSerial::line_people = "";
QString input_person_sampleSerial::line_serial = "";

input_person_sampleSerial::~input_person_sampleSerial()
{
    delete ui;
}

void input_person_sampleSerial:: slot_keyNumPressed(){
    QString tmpstr=sender()->objectName();
    tmpstr.remove("b_");
    QString current_text;
    if(ui->lineEdit_people->hasFocus()){
        current_text = ui->lineEdit_people->text();
        current_text += tmpstr;
        if(current_text.size() > 6) return;
        ui->lineEdit_people->setText(current_text);
        line_people = current_text;
    }else if (ui->lineEdit_serial->hasFocus()){
        current_text = ui->lineEdit_serial->text();
        current_text += tmpstr;
        if(current_text.size() > 13) return;
       ui->lineEdit_serial->setText(current_text);
       line_serial = current_text;
    }
}


void input_person_sampleSerial::on_b_return_clicked()
{
    ui->lineEdit_people->show();
    ui->lineEdit_serial->show();
    ui->label->show();
    ui->label_2->show();
    ui->lineEdit_people->setText("");
    ui->lineEdit_serial->setText("");
    this->close();
}

void input_person_sampleSerial::on_b_ok_clicked()
{
    if(ui->lineEdit_people->hasFocus()){
        emit transmit_data(ui->lineEdit_people->text());
    }else if (ui->lineEdit_serial->hasFocus()){
        emit transmit_data(ui->lineEdit_serial->text());
    }
    on_b_return_clicked();
}

void input_person_sampleSerial::just_show_people(){
    ui->lineEdit_serial->hide();
    ui->label_2->hide();
    this->showFullScreen();
}

void input_person_sampleSerial::just_show_sample(){
    ui->lineEdit_people->hide();
    ui->label->hide();
    this->showFullScreen();
}

void input_person_sampleSerial::on_pushButton_clicked()
{
    QString current_text;
    if(ui->lineEdit_people->hasFocus()){
        current_text = ui->lineEdit_people->text();
        current_text.chop(1);
        ui->lineEdit_people->setText(current_text);
        line_people = current_text;
    }else if (ui->lineEdit_serial->hasFocus()){
       current_text = ui->lineEdit_serial->text();
       current_text.chop(1);
       ui->lineEdit_serial->setText(current_text);
       line_serial = current_text;
    }
}
