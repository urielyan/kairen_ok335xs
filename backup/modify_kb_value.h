#ifndef MODIFY_KB_VALUE_H
#define MODIFY_KB_VALUE_H

#include <QWidget>

namespace Ui {
class modify_kb_value;
}

class AbstractDataSave;
class modify_kb_value : public QWidget
{
    Q_OBJECT

public:
    explicit modify_kb_value(QWidget *parent = 0);
    ~modify_kb_value();
    void show_and_refresh();

private slots:
    void slot_keynumberPressed();

    void on_b_abandon_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_b_point_clicked();

    void on_b_negative_clicked();

    void on_b_backspace_clicked();

    void on_b_sure_clicked();

private:
    Ui::modify_kb_value *ui;

    AbstractDataSave *p_mySettings;
};

#endif // MODIFY_KB_VALUE_H
