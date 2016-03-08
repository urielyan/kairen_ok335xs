#include "date_widget.h"
#include "ui_date_widget.h"
#include <QDate>
#include<QTime>
#include<QTimer>
#include<QDebug>
#include <global.h>
date_widget::date_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::date_widget)
{
    ui->setupUi(this);
    ui->label_2->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->label->setText(QDate::currentDate().toString("yyyy.MM.dd"));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
    timer->start(1000);
    this->setSizeIncrement(parent->size().width()/2,parent->size().height()/2);

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
      }
}

date_widget::~date_widget()
{
    delete ui;
}
void date_widget::update_time(){
    ui->label_2->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->label->setText(QDate::currentDate().toString("yyyy.MM.dd"));
}
