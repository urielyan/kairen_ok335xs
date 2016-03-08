#ifndef COUNTDATAQUERY_H
#define COUNTDATAQUERY_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class countdataquery;
}

class countdataquery : public QWidget
{
    Q_OBJECT

public:
    explicit countdataquery(QWidget *parent = 0);
    ~countdataquery();
    void show_and_refresh();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::countdataquery *ui;
    int display_count;
    //QSettings count_data;
};

#endif // COUNTDATAQUERY_H
