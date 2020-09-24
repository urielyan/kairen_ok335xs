#ifndef COUNT_KB_VALUE_H
#define COUNT_KB_VALUE_H

#include <QWidget>

namespace Ui {
class count_kb_value;
}

class AbstractDataSave;
class spectrum_painter;
class count_kb_value : public QWidget
{
    Q_OBJECT

public:
    explicit count_kb_value(QWidget *parent = 0);
    ~count_kb_value();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::count_kb_value *ui;

    //record the kbr,work_curve,number and datatime;
    static int count_record;

    //query_s_count_data *query_s_count_d;
    AbstractDataSave *p_mySettings;

    spectrum_painter *spec_painter;

    QString countKbrValue(int judge_which);
    void printer_result(QVector<int> count_input_s_index);
};

#endif // COUNT_KB_VALUE_H
