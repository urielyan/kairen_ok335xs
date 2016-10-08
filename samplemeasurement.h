#ifndef SAMPLEMEASUREMENT_H
#define SAMPLEMEASUREMENT_H

#include <QWidget>
#include <QString>
#include <QDateTime>
#include <QTimer>

namespace Ui {
class sampleMeasurement;
}

class showsamplemeasurement;
class countingMeasurement;
class AbstractDataSave;
class input_person_sampleSerial;
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

    void slotReturnClicked();

    void slotStartClicked();

    int slotStopClicked();

    void on_pushButton_4_clicked();

    void on_b_input_serial_clicked();

private:
    Ui::sampleMeasurement *ui;
    QTimer *timer_measurement;
    int count,change_count,second,change_second,work_queue,flag;
    showsamplemeasurement *showsm;
    countingMeasurement *counting_measurement;
    input_person_sampleSerial *input_serial;
    AbstractDataSave *p_mySettings;

    void all_combox_disabled(bool);
    void printer_result();
};

#endif // SAMPLEMEASUREMENT_H
