#ifndef INSPECTOSCOPE_H
#define INSPECTOSCOPE_H

#include <QWidget>

namespace Ui {
class inspectoscope;
}

class inspectoscope : public QWidget
{
    Q_OBJECT

public:
    explicit inspectoscope(QWidget *parent = 0);
    ~inspectoscope();
signals:
    void stop_all_inspectoscope();

private slots:
    void receive_time(int);
    void receive_count_time(int);
    void on_pushButton_clicked();

private:
    Ui::inspectoscope *ui;
    int spectrument_flag;
};

#endif // INSPECTOSCOPE_H
