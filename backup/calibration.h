#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>

namespace Ui {
class calibration;
}

class calibrationmeasurement;
class WinInputSPercentage;
class count_kb_value;
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

   // void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::calibration *ui;
    calibrationmeasurement *cm;
    WinInputSPercentage *inputsp;
    //query_s_count_data *query_s_count;
    count_kb_value *count_kb;
};

#endif // CALIBRATION_H
