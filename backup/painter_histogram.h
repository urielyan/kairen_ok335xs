#ifndef PAINTER_HISTOGRAM_H
#define PAINTER_HISTOGRAM_H

#include "global.h"
#include <QWidget>

//48
#define SPECTRUM_PAINTER_WIDTH  48

namespace Ui {
  class painter_histogram;
}

class painter_histogram : public QWidget
{
  Q_OBJECT

public:
  explicit painter_histogram(QWidget *parent = 0);
  ~painter_histogram();
  void spectrum_show_and_update(int);
  void just_updae();
  static int spectrum_data[SPECTRUM_PAINTER_WIDTH];
protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

    void on_b_forword_clicked();

    void on_b_back_clicked();

private:
  Ui::painter_histogram *ui;
  int different_display_color;
};

#endif // PAINTER_HISTOGRAM_H
