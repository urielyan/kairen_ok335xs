#include <QDebug>

#include "input_person_sampleserial.h"
#include "ui_input_person_sampleserial.h"
#include "global.h"

#include <QString>

input_person_sampleSerial *input_person_sampleSerial::instance()
{
  static input_person_sampleSerial _instance;
  return &_instance;
}

input_person_sampleSerial::input_person_sampleSerial(QWidget *parent) :
    QDialog(parent), ui(new Ui::input_person_sampleSerial)
{
    ui->setupUi(this);

    QList<QPushButton *> allPButtons = this->findChildren<QPushButton *>();
    for(int i=0;i<allPButtons.count();i++){
        allPButtons[i]->setFocusPolicy(Qt::NoFocus);//设置所有按钮没有焦点
        QString tmpstr=allPButtons[i]->objectName();
        if(tmpstr.length()==3){
            QObject::connect(allPButtons[i],SIGNAL(clicked()),this,SLOT(slotNumKeyPressed()));
        }
    }

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(slotBackspaceClicked()));
}

input_person_sampleSerial::~input_person_sampleSerial()
{
    delete ui;
}

QString input_person_sampleSerial::getSample()
{
    return ui->lineEdit_serial->text();
}

QString input_person_sampleSerial::getPeople()
{
    return ui->lineEdit_people->text();
}

void input_person_sampleSerial:: slotNumKeyPressed()
{
    QString tmpstr=sender()->objectName();
    tmpstr.remove("b_");
    QString current_text;
    if(ui->lineEdit_people->hasFocus()){
        current_text = ui->lineEdit_people->text();
        current_text += tmpstr;
        if(current_text.size() > 6) return;
        ui->lineEdit_people->setText(current_text);
    }else if (ui->lineEdit_serial->hasFocus()){
        current_text = ui->lineEdit_serial->text();
        current_text += tmpstr;
        if(current_text.size() > 13) return;
       ui->lineEdit_serial->setText(current_text);
    }
}

void input_person_sampleSerial::showPeopleSample()
{
    initWidget();
}

void input_person_sampleSerial::showPeople()
{
    initWidget();

    ui->lineEdit_serial->hide();
    ui->label_2->hide();
}

void input_person_sampleSerial::showSample()
{

    ui->lineEdit_people->hide();
    ui->label->hide();
}

void input_person_sampleSerial::on_b_return_clicked()
{
    initWidget();
    this->reject();
}

void input_person_sampleSerial::on_b_ok_clicked()
{
    if(ui->lineEdit_people->hasFocus()){
        emit transmit_data(ui->lineEdit_people->text());
    }else if (ui->lineEdit_serial->hasFocus()){
        emit transmit_data(ui->lineEdit_serial->text());
    }
    this->accept();
}

void input_person_sampleSerial::initWidget()
{
    ui->lineEdit_people->show();
    ui->lineEdit_serial->show();
    ui->label->show();
    ui->label_2->show();
    ui->lineEdit_people->setText("");
    ui->lineEdit_serial->setText("");
}

void input_person_sampleSerial::slotBackspaceClicked()
{
    QString current_text;
    if(ui->lineEdit_people->hasFocus()){
        current_text = ui->lineEdit_people->text();
        current_text.chop(1);
        ui->lineEdit_people->setText(current_text);
    }else if (ui->lineEdit_serial->hasFocus()){
       current_text = ui->lineEdit_serial->text();
       current_text.chop(1);
       ui->lineEdit_serial->setText(current_text);
    }
}
