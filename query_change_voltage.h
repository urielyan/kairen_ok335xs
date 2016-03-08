#ifndef QUERY_CHANGE_VOLTAGE_H
#define QUERY_CHANGE_VOLTAGE_H

#include <QWidget>

namespace Ui {
  class query_change_voltage;
}

class query_change_voltage : public QWidget
{
  Q_OBJECT

public:
  explicit query_change_voltage(QWidget *parent = 0);
  ~query_change_voltage();
  void show_and_refresh();
private slots:
  void on_pushButton_clicked();

private:
  Ui::query_change_voltage *ui;
};

#endif // QUERY_CHANGE_VOLTAGE_H
