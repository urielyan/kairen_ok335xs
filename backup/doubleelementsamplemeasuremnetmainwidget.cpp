#include "doubleelementsamplemeasuremnetmainwidget.h"
#include "ui_doubleelementsamplemeasuremnetmainwidget.h"

DoubleElementSampleMeasuremnetMainWidget::DoubleElementSampleMeasuremnetMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleElementSampleMeasuremnetMainWidget)
{
    ui->setupUi(this);
}

DoubleElementSampleMeasuremnetMainWidget::~DoubleElementSampleMeasuremnetMainWidget()
{
    delete ui;
}
