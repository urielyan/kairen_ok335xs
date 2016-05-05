#include "inspectoscope.h"
#include "ui_inspectoscope.h"
#include "global.h"

#define SPECTRUMENT_MEANUREMENT_DEFEAT 0
#define SPECTRUMENT_MEANUREMENT_SUCCESS 1
inspectoscope::inspectoscope(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inspectoscope)
{
    ui->setupUi(this);
    ui->widget->hide();
    ui->widget_2->hide();
    ui->label_state->setText("");

    this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    QList<QLabel *> labellist = this->findChildren<QLabel *>();
    for (int i = 0; i < labellist.count(); ++i) {
        labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      }
    ui->label_title->setFont(QFont(FONT_NAME, FONT_SIZE*1.5 ,QFont::Normal));
    //ui->label->setObjectName("title");
}

inspectoscope::~inspectoscope()
{
    delete ui;
}
void inspectoscope::receive_count_time(int time_val){
  //计数测量数据接受站
    ui->widget_2->show();
    ui->label_2sec->setText(QString::number(time_val));
    if(time_val == SUCCESS_COUNT){
        ui->widget_2->hide();
        if(spectrument_flag == SPECTRUMENT_MEANUREMENT_SUCCESS){
            ui->label_state->setText("自检完成，仪器正常");
          }else if(spectrument_flag == SPECTRUMENT_MEANUREMENT_DEFEAT){
            ui->label_state->setText("能谱峰位测量未通过,计数测量通过\n自检完成，仪器不正常");
          }
    }else if(time_val == DEFEAT_COUNT){
        ui->widget_2->hide();
        if(spectrument_flag == SPECTRUMENT_MEANUREMENT_SUCCESS){
            ui->label_state->setText("能谱峰位测量通过,计数测量未通过\n自检完成，仪器不正常");
          }else if(spectrument_flag == SPECTRUMENT_MEANUREMENT_DEFEAT){
            ui->label_state->setText("能谱峰位测量未通过,计数测量未通过\n自检完成，仪器不正常");
          }
    }
}
void inspectoscope::receive_time(int time_val){
  //能谱峰位测量数据接受站
    ui->widget->show();
    ui->label_1sec->setText(QString::number(time_val));
    if(time_val == SUCCESS_SPECTOSCOPE){
        ui->widget->hide();
        ui->label_state->setText("能谱峰位测量通过");
        spectrument_flag = SPECTRUMENT_MEANUREMENT_SUCCESS;
    }else if(time_val == DEFEAT_SPECTOSCOPE){
        ui->widget->hide();
        ui->label_state->setText("能谱峰位测量未通过");
        spectrument_flag = SPECTRUMENT_MEANUREMENT_DEFEAT;
    }
}

void inspectoscope::on_pushButton_clicked()
{
    ui->label_1sec->setText("");
    ui->label_2sec->setText("");
    ui->widget->hide();
    ui->widget_2->hide();
    ui->label_state->setText("");
    emit stop_all_inspectoscope();
    this->close();
}
