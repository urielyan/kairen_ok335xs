#ifndef SHOWCALIBRATEMEASUREMENT_H
#define SHOWCALIBRATEMEASUREMENT_H

#include <QWidget>

namespace Ui {
class showcalibratemeasurement;
}

class showcalibratemeasurement : public QWidget
{
    Q_OBJECT

public:
    explicit showcalibratemeasurement(QWidget *parent = 0);
    ~showcalibratemeasurement();
    void add_calibratemeasurement_data(int count,QString calibrate_count , QString reference_count);
    void clear_label();
private slots:
    void on_pushButton_clicked();

private:
    Ui::showcalibratemeasurement *ui;
};

#endif // SHOWCALIBRATEMEASUREMENT_H
