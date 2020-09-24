#ifndef DATAQUERY_H
#define DATAQUERY_H

#include <QWidget>

namespace Ui {
class dataquery;
}

class samplemeasurementquery;
class WinSqlDataQuery;
class caibrateresultquery;
class countdataquery;
class dataquery : public QWidget
{
    Q_OBJECT

public:
    explicit dataquery(QWidget *parent = 0);
    ~dataquery();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::dataquery *ui;
    countdataquery *showcountdataquery;
    caibrateresultquery *csquery;
    samplemeasurementquery *p_sampleQueryByQSettings;
    WinSqlDataQuery *p_sampleQueryByDatabase;
};

#endif // DATAQUERY_H
