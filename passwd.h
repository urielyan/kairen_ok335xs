
#ifndef PASSWD_H
#define PASSWD_H

//#include <QSettings>
#include <QWidget>
#include "systemsetup.h"
#include "hide_system.h"

#define COUNT_VOLTAGE_MAX 700
#define COUNT_VOLTAGE_MIN 500

#define DISPLAY_ALTER_PASSWD 1
#define SETUP_COUNT_VOLTAGE     7
#define SETUP_LIGHT_VOLTAGE     8
#define SETUP_LIGHT_ELECTRICITY 9

namespace Ui {
class passwd;
}

class passwd : public QWidget
{
    Q_OBJECT

public:
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
    QSettings mysettings;
    int stop_all_measurement();
};

#endif // PASSWD_H
