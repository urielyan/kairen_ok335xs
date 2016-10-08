#ifndef PROPORTION_H
#define PROPORTION_H

#include <QWidget>

namespace Ui {
class proportion;
}

class AbstractDataSave;
class proportion : public QWidget
{
    Q_OBJECT

public:
    explicit proportion(QWidget *parent = 0);
  void showAndInit();
    ~proportion();

private slots:
    void on_pushButton_3_clicked();

    void slot_keyNumPressed();


    void on_b_sure_clicked();

    void on_b_backspace_clicked();

    void on_b_clear_clicked();

    void on_b_point_clicked();

private:
    Ui::proportion *ui;

    AbstractDataSave *p_mySettings;
};

#endif // PROPORTION_H
