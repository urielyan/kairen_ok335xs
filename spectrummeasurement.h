#ifndef SPECTRUMMEASUREMENT_H
#define SPECTRUMMEASUREMENT_H

#include <QWidget>
#include <QTimer>
#include "com.h"
#include "countingmeasurement.h"
#include "painter_histogram.h"

namespace Ui {
class spectrummeasurement;
}

class spectrummeasurement : public QWidget
{
    Q_OBJECT

public:
    explicit spectrummeasurement(QWidget *parent = 0);
    void try_inspectoscope();
    ~spectrummeasurement();

signals:
    void transmit(int);
    void transmit_count_time(int);
    void transmit_stop_auto_count();
    void transmit_steady_summit_result(double);
    void transmit_move_sliding(bool,bool);


public slots:
    void on_pushButton_5_clicked();

    int on_pushButton_4_clicked();

    int on_pushButton_2_clicked();
    int stop_measurement_no_transmit();
    void start_steady_summit();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void changetable();


    void receive_transmit_count_time(int);
    void stop_inspectoscope();

    void on_pushButton_6_clicked();
    void set_sliding_disabled(bool,bool);

private:
        Ui::spectrummeasurement *ui;

        int summit_value;
        QString max_volumn,max_data;
        size_t row,column,flag,try_inspectoscope_flag;

        QTimer *timer;

        Communciation_Com *spectrum_com;
        countingMeasurement *countmeas;
        painter_histogram * painter_result;

        void initTableWidget();
        void disable_pushbutton(bool);
        void printer_result();
        void printPicture();
};

#endif // SPECTRUMMEASUREMENT_H
