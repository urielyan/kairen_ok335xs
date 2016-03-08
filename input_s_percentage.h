#ifndef INPUT_S_PERCENTAGE_H
#define INPUT_S_PERCENTAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class input_S_percentage;
}

class input_S_percentage : public QWidget
{
    Q_OBJECT

public:
    explicit input_S_percentage(QWidget *parent = 0);
    ~input_S_percentage();

    void clear_all_tablewidget();
    //int judge_is_available();

private slots:
    void slot_keyNumPressed();

    void on_b_abandon_clicked();

    void on_b_clear_clicked();

    void on_b_backspace_clicked();

    void on_b_point_clicked();

    void on_b_sure_clicked();

private:
    Ui::input_S_percentage *ui;
    QSettings mysettings;
};

#endif // INPUT_S_PERCENTAGE_H
