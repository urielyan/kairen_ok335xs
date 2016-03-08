#ifndef SAMPLE_DATA_QUERY_H
#define SAMPLE_DATA_QUERY_H

#include <QWidget>
#include <QSqlTableModel>
#include "input_person_sampleserial.h"
namespace Ui {
class sample_data_query;
}

class sample_data_query : public QWidget
{
    Q_OBJECT

public:
    explicit sample_data_query(QWidget *parent = 0);
    ~sample_data_query();

    void show_and_refresh();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_b_datetime_clicked();

    void on_b_datetime_disorder_clicked();

    void wait_input_result(QString);

    void on_b_sample_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    int query_flag;
    Ui::sample_data_query *ui;
    QSqlTableModel *model;
    input_person_sampleSerial *input_serial;
    void init_tableview();
};

#endif // SAMPLE_DATA_QUERY_H
