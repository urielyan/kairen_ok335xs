#ifndef SAMPLEMEASUREMENTQUERY_H
#define SAMPLEMEASUREMENTQUERY_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class samplemeasurementquery;
}

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
    Ui::samplemeasurementquery *ui;
    int current_count;
    void refresh_widget(int);
    QSettings mysettings;
};

#endif // SAMPLEMEASUREMENTQUERY_H
