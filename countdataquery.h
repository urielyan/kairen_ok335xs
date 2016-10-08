#ifndef COUNTDATAQUERY_H
#define COUNTDATAQUERY_H

#include <QWidget>

namespace Ui {
class countdataquery;
}

class AbstractDataSave;
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
    AbstractDataSave *p_mySettings;

    int display_count;
};

#endif // COUNTDATAQUERY_H
