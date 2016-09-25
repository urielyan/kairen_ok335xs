#ifndef QUERY_S_COUNT_DATA_H
#define QUERY_S_COUNT_DATA_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QSettings>

namespace Ui {
class query_s_count_data;
}

class query_s_count_data : public QWidget
{
    Q_OBJECT

public:
    explicit query_s_count_data(QWidget *parent = 0);
    ~query_s_count_data();
    void add_s_count_data(int ,QString);//TODO:: useless
    void clear_data();
    void show_and_refresh();
    QString get_kbr_a012(int);

private slots:
    void on_pushButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

private:
    Ui::query_s_count_data *ui;
    QTableWidgetItem *tmpitem[12];
    QSettings *p_mysettings;
};

#endif // QUERY_S_COUNT_DATA_H
