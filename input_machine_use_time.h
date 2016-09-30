#ifndef INPUT_MACHINE_USE_TIME_H
#define INPUT_MACHINE_USE_TIME_H

#include <QWidget>

namespace Ui {
  class input_machine_use_time;
}

class AbstractDataSave;
class input_machine_use_time : public QWidget
{
  Q_OBJECT

public:
  explicit input_machine_use_time(QWidget *parent = 0);
  ~input_machine_use_time();
  void input_product_serial();
  void input_decode_serial();

private slots:
  void on_b_ok_clicked();

  void on_b_return_clicked();
  void slot_keyNumPressed();

private:
  Ui::input_machine_use_time *ui;
  //QSettings mysettings;
  AbstractDataSave *p_mySettings;
};

#endif // INPUT_MACHINE_USE_TIME_H
