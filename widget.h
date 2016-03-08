#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>

#include "countingmeasurement.h"
#include "spectrummeasurement.h"
#include "dataquery.h"
#include "samplemeasurement.h"
#include "passwd.h"
#include "inspectoscope.h"
#include "com.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    //void start_try_inspectoscope_count();

signals:
    void transmit_stop_auto_count();
    void change_sliding_disabled(bool,bool);
private slots:
    void on_b1_clicked();

    void on_b4_clicked();

    void on_b5_clicked();

    void on_b2_clicked();

    void on_b3_clicked();
    void update_count_5_data();
    void auto_start_count_measument();

    int on_b6_clicked();
    void set_sliding_disabled(bool,bool);
    void judge_spectrument_measurement_result(double);

private:
    void get_slide_current_position();
    Ui::Widget *ui;
    countingMeasurement *cm;
    countingMeasurement *cm_auto;
    spectrummeasurement *sm;
    dataquery *dq;
    sampleMeasurement *samplem;
    passwd *pw;
    inspectoscope *inspectoscope_measurement;
    QSettings mysettings;
};

#endif // WIDGET_H
