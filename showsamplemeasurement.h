#ifndef SHOWSAMPLEMEASUREMENT_H
#define SHOWSAMPLEMEASUREMENT_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSettings>

namespace Ui {
  class showsamplemeasurement;
}

class showsamplemeasurement : public QWidget
{
  Q_OBJECT

public:
  explicit showsamplemeasurement(QWidget *parent = 0);
  ~showsamplemeasurement();
  void add_data(int,QString,int);
  void show_calculate_storage(QString);
  void hide_lable(bool);
  void clear_tablewidget();
  int get_real_curve();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  Ui::showsamplemeasurement *ui;
  QTableWidgetItem *itemlist[50];
  double sum;
  int real_curve;
  QSqlTableModel *model;
  QSqlDatabase db ;
  QSettings mysettings;

  void printer_result();
};

#endif // SHOWSAMPLEMEASUREMENT_H
