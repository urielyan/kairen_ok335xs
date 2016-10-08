#ifndef CALIBRATIONMEASUREMENT_H
#define CALIBRATIONMEASUREMENT_H

#include <QWidget>
#include <QTimer>


class showcalibratemeasurement;
namespace Ui {
class calibrationmeasurement;
}

class AbstractDataSave;
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
    void slotStartClicked();

    void on_pushButton_4_clicked();

    int slotStopClicked();

    void slotReturnClicked();

private:
    int second,count_flag,button_flag;

    Ui::calibrationmeasurement *ui;
    showcalibratemeasurement *showcalibratemeasure;
    QTimer *timer;
    AbstractDataSave *p_mySettings;

    void disable_button(bool);
};

#endif // CALIBRATIONMEASUREMENT_H
