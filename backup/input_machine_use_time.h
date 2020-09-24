#ifndef INPUT_MACHINE_USE_TIME_H
#define INPUT_MACHINE_USE_TIME_H

#include <QWidget>

/**
讨债程序是这样的：
  每分钟都会检测当前设备的程序是否已经到期；
   若到期会弹出一个对话框，输入序号1111111 会使程序永远可用
   可在管理员菜单栏里设置 使用时间，需要输入七位数，最后一位有效，前六位代表产品序号，最后一位为可使用时间（单位为月）
  */
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

  void on_clearButton_clicked();

  void on_backspaceButton_clicked();

private:
  Ui::input_machine_use_time *ui;

  AbstractDataSave *p_mySettings;

  //序列号最后一位
  int seirialSumLastNumber(long code);

  QString serialAdd67896789(QString serial);
};

#endif // INPUT_MACHINE_USE_TIME_H
