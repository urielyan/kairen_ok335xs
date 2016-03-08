#ifndef CALIBRATIONMEASUREMENT_H
#define CALIBRATIONMEASUREMENT_H

#include <QWidget>
#include <QTimer>
#include "showcalibratemeasurement.h"
#include "query_s_count_data.h"
#include "com.h"
#include <QSettings>

namespace Ui {
class calibrationmeasurement;
}

class calibrationmeasurement : public QWidget
{
    Q_OBJECT

public:
    static int count;//record the time to measurement
    explicit calibrationmeasurement(QWidget *parent = 0);
    ~calibrationmeasurement();
signals:
    void transmit_stop_auto_count();

private slots:
    void doing_measurement();
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    int on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::calibrationmeasurement *ui;
    QTimer *timer;
    int second,count_flag,button_flag;
    showcalibratemeasurement *showcalibratemeasure;
    query_s_count_data *query_s_count_data_in_this;
    com *calibrate_com;
    void disable_button(bool);
    QSettings mysettings;
};

#endif // CALIBRATIONMEASUREMENT_H
