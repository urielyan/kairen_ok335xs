#ifndef SPECTRUM_PAINTER_H
#define SPECTRUM_PAINTER_H

#include <QWidget>

namespace Ui {
  class spectrum_painter;
}

class spectrum_painter : public QWidget
{
  Q_OBJECT

public:
  explicit spectrum_painter(QWidget *parent = 0);
  ~spectrum_painter();
  void show_special_curve(int,QMap<int,QString>,QString);

protected:
    void paintEvent(QPaintEvent *);

private slots:
  void on_pushButton_clicked();

private:
  Ui::spectrum_painter *ui;
  double v1,v2,v3;
  int curve;
  double max_data_y,max_data_x;
  int multiple_size;

};

#endif // SPECTRUM_PAINTER_H
