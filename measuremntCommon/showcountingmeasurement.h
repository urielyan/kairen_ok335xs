#ifndef SHOWCOUNTINGMEASUREMENT_H
#define SHOWCOUNTINGMEASUREMENT_H

#include <QWidget>

namespace Ui {
class showcountingmeasurement;
}

class     AbstractDataSave;
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
    AbstractDataSave *p_mySettings;
};

#endif // SHOWCOUNTINGMEASUREMENT_H
