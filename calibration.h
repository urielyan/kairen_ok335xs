#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include "calibrationmeasurement.h"
#include "input_s_percentage.h"
#include "query_s_count_data.h"
#include "count_kb_value.h"

namespace Ui {
class calibration;
}

class calibration : public QWidget
{
    Q_OBJECT

public:
    explicit calibration(QWidget *parent = 0);
    ~calibration();

signals:
    void transmit_stop_auto_count();

private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::calibration *ui;
    calibrationmeasurement *cm;
    input_S_percentage *inputsp;
    query_s_count_data *query_s_count;
    count_kb_value *count_kb;
};

#endif // CALIBRATION_H
