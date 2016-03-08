#ifndef DATE_WIDGET_H
#define DATE_WIDGET_H

#include <QWidget>
#include<QTime>
namespace Ui {
class date_widget;
}

class date_widget : public QWidget
{
    Q_OBJECT

public:
    explicit date_widget(QWidget *parent = 0);

    ~date_widget();
private slots:
    void update_time();
private:
    Ui::date_widget *ui;

};

#endif // DATE_WIDGET_H
