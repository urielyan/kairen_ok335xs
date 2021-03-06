
#ifndef SYSTEMSETUP_H
#define SYSTEMSETUP_H
#include <QWidget>

namespace Ui {
  class systemsetup;
}

class setupdatetime;
class calibration;
class modify_kb_value;
class countingMeasurement;
class spectrummeasurement;
class communication_help;
class systemsetup : public QWidget
{
  Q_OBJECT

public:
  explicit systemsetup(QWidget *parent = 0);
  ~systemsetup();

signals:
  void transmit_stop_auto_count();
  void change_sliding_disabled(bool,bool);
  void change_widget_sliding(bool,bool);
  void show_alterpasswd(int);

private slots:
  void on_pushButton_2_clicked();
  void on_pushButton_clicked();
  void on_pushButton_6_clicked();
  void on_pushButton_4_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_7_clicked();
  void on_pushButton_9_clicked();
  void on_pushButton_5_clicked();

private:
  Ui::systemsetup *ui;
  setupdatetime *setdt;
  calibration *calibrate;
  modify_kb_value *modify_kb;
  countingMeasurement *count_mea;
  spectrummeasurement *specture_mea;
  communication_help *communicate_and_help;
};

#endif // SYSTEMSETUP_H
