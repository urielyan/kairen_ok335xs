#ifndef ISPREHEAT_H
#define ISPREHEAT_H

#include <QWidget>
#include <QLabel>

#define NEED_START_SAMPLING 9999
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
    bool blink_flag;

    QTimer *timer;
    QTimer *timer2;

    QLabel m_labelIsPreheat;
    QLabel m_labelIsSampling;
};

#endif // ISPREHEAT_H
