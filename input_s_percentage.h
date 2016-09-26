#ifndef INPUT_S_PERCENTAGE_H
#define INPUT_S_PERCENTAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class input_S_percentage;
}

class AbstractDataSave;
class WinInputSPercentage : public QWidget
{
    Q_OBJECT

public:
    explicit WinInputSPercentage(QWidget *parent = 0);
    ~WinInputSPercentage();

    void clear_all_tablewidget();
    void showAndUpdateData();
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
    //QSettings mysettings;
    AbstractDataSave *p_mySettings;

    void initTableWidget();
    void initTableWidgetData();
    void initSignalSlotConnect();
};

#endif // INPUT_S_PERCENTAGE_H
