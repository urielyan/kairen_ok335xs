#ifndef SAMPLEMEASUREMENTQUERY_H
#define SAMPLEMEASUREMENTQUERY_H

#include <QWidget>

namespace Ui {
class samplemeasurementquery;
}

class AbstractDataSave;
class samplemeasurementquery : public QWidget
{
    Q_OBJECT

public:
    explicit samplemeasurementquery(QWidget *parent = 0);
    ~samplemeasurementquery();
    void show_and_refresh();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_prev_clicked();

private:
    int current_count;

    Ui::samplemeasurementquery *ui;
    AbstractDataSave *p_mySettings;

    void refresh_widget(int);
};

#endif // SAMPLEMEASUREMENTQUERY_H
