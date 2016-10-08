#ifndef ISPREHEAT_H
#define ISPREHEAT_H

#include <QWidget>

#define NEED_START_SAMPLING 9999
namespace Ui {
class ispreheat;
}

class ispreheat : public QWidget
{
    Q_OBJECT

public:
    explicit ispreheat(QWidget *parent = 0);
    ~ispreheat();
    int global_ispreheat;
    void change_label_content(QString);
    static int is_sampling_num;
private slots:
    void change_label_content();
    void check_ispreheat();

private:
    Ui::ispreheat *ui;
    QTimer *timer;
    QTimer *timer2;
    bool blink_flag;
    static int fd;
};

#endif // ISPREHEAT_H
