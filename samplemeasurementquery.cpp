#include "samplemeasurementquery.h"
#include "ui_samplemeasurementquery.h"
#include "global.h"

//extern QSettings mysettings;


samplemeasurementquery::samplemeasurementquery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::samplemeasurementquery)
{
    ui->setupUi(this);

    current_count = mysettings.value("sample_count").toInt();
    refresh_widget(current_count);

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));

    //ui->label->setObjectName("title");
}

samplemeasurementquery::~samplemeasurementquery()
{
    delete ui;
}

void samplemeasurementquery::show_and_refresh(){
    refresh_widget(mysettings.value("sample_count").toInt());
    this->showFullScreen();
}
void samplemeasurementquery::on_pushButton_return_clicked()
{
    current_count = mysettings.value("sample_count").toInt();
    refresh_widget(current_count);
    this->close();
}

void samplemeasurementquery::on_pushButton_3_clicked()
{
    //next
    current_count++;
    refresh_widget(current_count);
}

void samplemeasurementquery::on_pushButton_prev_clicked()
{
    //previous
    current_count--;
    refresh_widget(current_count);
}

void samplemeasurementquery::refresh_widget(int count){
    if(count > mysettings.value("sample_count").toInt()){
        current_count--;
        return;
    }
    if(count <= 0){
        current_count++;
        return;
    }
    QStringList data_list = mysettings.value(QString("sample_data_%1").arg(count)).toString().split(";");
    if(data_list.size() != 6){
        mysettings.remove(QString("sample_data_%1").arg(count));
    }
    ui->label_curve->setText(data_list[0]);
    ui->label_datetime->setText(data_list[1]);
    ui->label_measurement_time->setText(data_list[2]);
    ui->label_repeat_time->setText(data_list[3]);
    ui->label_average->setText(data_list[4]);
    ui->label_deviation->setText(data_list[5]);
}
