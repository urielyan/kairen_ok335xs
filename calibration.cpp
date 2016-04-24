#include "calibration.h"
#include "ui_calibration.h"
#include "input_s_percentage.h"
#include "global.h"

#include <QMessageBox>

calibration::calibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calibration)
{
    ui->setupUi(this);
    cm = new calibrationmeasurement();
    inputsp = new WidgetInputSPercentage();
    query_s_count = new query_s_count_data();
    count_kb = new count_kb_value();
    connect(this,SIGNAL(transmit_stop_auto_count()),cm,SIGNAL(transmit_stop_auto_count()));

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      }
    ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
}

calibration::~calibration()
{
    delete count_kb;
    delete query_s_count;
    delete inputsp;
    delete cm;
    delete ui;
}

void calibration::on_pushButton_6_clicked()
{
    //return button
    this->close();
}

void calibration::on_pushButton_2_clicked()
{
    //2 calibrate measurement button
    cm->showFullScreen();
}

void calibration::on_pushButton_4_clicked()
{
    //4 input S%
    inputsp->showFullScreen();
}

void calibration::on_pushButton_clicked()
{
    //clear data include input_s_i and data
    QMessageBox msgbox;
    msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    msgbox.setText("您想清除所有标定数据么？");
    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Yes);
    int ret = msgbox.exec();

    if (QMessageBox::Yes == ret){
        inputsp->clear_all_tablewidget();
        query_s_count->clear_data();
        calibrationmeasurement::count = 0;
        QMessageBox box;
        msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
        box.setText("您已清除标定样的硫含量和标定测量数据");
        box.exec();
    }
}

void calibration::on_pushButton_3_clicked()
{
    query_s_count->show_and_refresh();
}

void calibration::on_pushButton_5_clicked()
{
    count_kb->showFullScreen();
}
