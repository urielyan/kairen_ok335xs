#ifndef SETUPDATETIME_H
#define SETUPDATETIME_H

#include <QWidget>

namespace Ui {
class setupdatetime;
}

class setupdatetime : public QWidget
{
    Q_OBJECT

public:
    explicit setupdatetime(QWidget *parent = 0);
    ~setupdatetime();
  void show_and_refresh();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_p_buttonPlus_clicked();

    void on_p_buttonSub_clicked();

private:
    Ui::setupdatetime *ui;
};

#endif // SETUPDATETIME_H
