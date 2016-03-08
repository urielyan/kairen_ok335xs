#ifndef SHOWCOUNTINGMEASUREMENT_H
#define SHOWCOUNTINGMEASUREMENT_H

#include <QWidget>
#include <QDateTime>
#include <QSettings>

namespace Ui {
class showcountingmeasurement;
}

class showcountingmeasurement : public QWidget
{
    Q_OBJECT

public:
    explicit showcountingmeasurement(QWidget *parent = 0);
    ~showcountingmeasurement();
   void adddata(int,QString);
   void clearall();
   float get_y();
   float get_average_x();

private slots:
    void on_pushButton_clicked();

private:
    Ui::showcountingmeasurement *ui;
     //QSettings count_data;
};

#endif // SHOWCOUNTINGMEASUREMENT_H
