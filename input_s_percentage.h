#ifndef INPUT_S_PERCENTAGE_H
#define INPUT_S_PERCENTAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class input_S_percentage;
}

class WidgetInputSPercentage : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetInputSPercentage(QWidget *parent = 0);
    ~WidgetInputSPercentage();

    void clear_all_tablewidget();
    //int judge_is_available();

private slots:
    void slot_keyNumPressed();

    void on_b_abandon_clicked();

    void on_b_clear_clicked();

    void on_b_backspace_clicked();

    void slotPointClicked();

    void on_b_sure_clicked();

private:
    Ui::input_S_percentage *ui;
    QSettings mysettings;

    void initTableWidget();
    void initSignalSlotConnect();
};

#endif // INPUT_S_PERCENTAGE_H
