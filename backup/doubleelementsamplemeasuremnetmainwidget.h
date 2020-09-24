#ifndef DOUBLEELEMENTSAMPLEMEASUREMNETMAINWIDGET_H
#define DOUBLEELEMENTSAMPLEMEASUREMNETMAINWIDGET_H

#include <QWidget>

namespace Ui {
class DoubleElementSampleMeasuremnetMainWidget;
}

class DoubleElementSampleMeasuremnetMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleElementSampleMeasuremnetMainWidget(QWidget *parent = 0);
    ~DoubleElementSampleMeasuremnetMainWidget();

private:
    Ui::DoubleElementSampleMeasuremnetMainWidget *ui;
};

#endif // DOUBLEELEMENTSAMPLEMEASUREMNETMAINWIDGET_H
