#include "showcalibratemeasurement.h"
#include "ui_showcalibratemeasurement.h"
#include "global.h"

#include <QDateTime>

showcalibratemeasurement::showcalibratemeasurement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showcalibratemeasurement)
{
    ui->setupUi(this);

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2 ,QFont::Normal));

    ui->pushButton->setFixedHeight(FONT_SIZE * 2);
    ui->pushButton->setFixedWidth(FONT_SIZE * 4);
    ui->label->setObjectName("title");
}

showcalibratemeasurement::~showcalibratemeasurement()
{
    delete ui;
}

void showcalibratemeasurement::on_pushButton_clicked()
{
    this->close();
}

void showcalibratemeasurement::add_calibratemeasurement_data(int count,QString calibrate_count , QString reference_count){
    ui->label_datatime->setText(QDateTime::currentDateTime().toString("MM-dd  hh:mm"));
    if(count <=12 && count >= 0)
        ui->label_number->setText(QString("第 %1# 标定样").arg(count));
    if(calibrate_count.size() == 5)
        ui->label_calibrate->setText(calibrate_count);
    if(reference_count.size() == 5)
        ui->label_reference->setText(reference_count);
}

void showcalibratemeasurement::clear_label(){
    ui->label_datatime->clear();
    ui->label_calibrate->clear();
    ui->label_reference->clear();
    ui->label_number->clear();
}
