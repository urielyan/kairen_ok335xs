#ifndef SAMPLEMEASUREMENT_H
#define SAMPLEMEASUREMENT_H

#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QTimer>

#include "showsamplemeasurement.h"
#include "com.h"
#include "countingmeasurement.h"
#include "input_person_sampleserial.h"
#include <QSettings>

namespace Ui {
class sampleMeasurement;
}

class sampleMeasurement : public QWidget
{
    Q_OBJECT

public:
    explicit sampleMeasurement(QWidget *parent = 0);
    ~sampleMeasurement();

signals:
    void transmit_stop_auto_count();

private slots:
    void doing_measurement();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    int on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_b_input_serial_clicked();

private:
    Ui::sampleMeasurement *ui;
    QTimer *timer_measurement;
    int count,change_count,second,change_second,work_queue,flag;
    showsamplemeasurement *showsm;
    countingMeasurement *counting_measurement;
    input_person_sampleSerial *input_serial;
    QSettings mysettings;

    void all_combox_disabled(bool);
    void printer_result();
};

#endif // SAMPLEMEASUREMENT_H
