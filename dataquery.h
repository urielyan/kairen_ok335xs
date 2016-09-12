#ifndef DATAQUERY_H
#define DATAQUERY_H

#include <QWidget>
#include "countdataquery.h"
#include "caibrateresultquery.h"
//#include "samplemeasurementquery.h"
#include "sample_data_query.h"

class samplemeasurementquery;
namespace Ui {
class dataquery;
}

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
    samplemeasurementquery *samplemeasurement_query;
    WinSqlDataQuery *p_sqlDataQuery;
};

#endif // DATAQUERY_H
