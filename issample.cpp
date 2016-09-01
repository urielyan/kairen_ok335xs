#include "issample.h"
#include "ui_issample.h"
#include  "global.h"

#include <QTimer>

extern int measurement_flag;

issample::issample(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::issample)
{
    ui->setupUi(this);

    global_is_sample = 1;

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(change_label_content()));
    timer->start(1000);

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
        labellist[i]->setMinimumHeight(FONT_SIZE + 2);
      }
}
int issample::global_is_sample;


issample::~issample()
{
    delete ui;
}
void issample::change_label_content(int i){
    global_is_sample = i;
}
void issample::change_label_content(){

    switch (global_is_sample) {
    case REFERENCE_BE_LOCATON:
        ui->label->setText(REFERENCE_BE_LOCATON_TEXT);
        break;
    case WAIT_BE_LOCATION:
        ui->label->setText(WAIT_BE_LOCATION_TEXT);
        break;
    case MACHINE_MALFUNCTION:
        ui->label->setText(MACHINE_MALFUNCTION_TEXT);
        break;
    case SLIDING_PLATE_NO_CHANGE:
        ui->label->setText(SLIDING_PLATE_NO_CHANGE_TEXT);
        //ui->label->setStyleSheet("background-image:");
        break;
    default:
        break;
    }
#ifdef DEBUG
    ui->label->setText(ui->label->text().append(QString::number(measurement_flag)));
#endif
}
