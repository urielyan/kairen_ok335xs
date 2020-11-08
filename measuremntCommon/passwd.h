
#ifndef PASSWD_H
#define PASSWD_H

#include <QWidget>

#define SETUP_ALTER_PASSWD 1
#define SETUP_COUNT_VOLTAGE     7
#define SETUP_LIGHT_VOLTAGE     8
#define SETUP_LIGHT_ELECTRICITY 9

namespace Ui {
class passwd;
}

class hide_system;
class systemsetup;
class AbstractDataSave;
class input_machine_use_time;
class passwd : public QWidget
{
    Q_OBJECT

public:
  enum ENUM_INPUT_MAX{
    //unit : v
    enumMaxCountVoltage = 1500,
    enumMinCountVoltage = 500,

    enumMaxLightVoltage = 9999,
    enumMinLightVoltage = 3000,

    //unit : uA
    enumMaxLightCurrent = 1000,
    enumMinLightCurrent = 50,
  };
    explicit passwd(QWidget *parent = 0);
    ~passwd();
    void showpass();
    int set_count_voltage(int);

signals:
    void transmit_stop_auto_count();
    void change_sliding_disabled(bool,bool);
    void change_widget_sliding(bool,bool);
    void transmit_steady_summit_result(double);
    void start_steady_summit_measurement();

private slots:
    void slotBackSpaceClicked();
    void slotClearClicked();
    void slot_keyNumPressed();

    void on_b_return_clicked();

    void alterpasswd_show(int);

    void on_b_ok_clicked();

    void alterpasswd();

private:
    Ui::passwd *ui;
    systemsetup *ss;
    hide_system *hs;
    int flag;
    input_machine_use_time *input;

    AbstractDataSave *p_mySettings;

    int stop_all_measurement();
};

#endif // PASSWD_H
