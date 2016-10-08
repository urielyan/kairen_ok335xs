#include <QPainter>
#include <QDebug>

#include "painter_histogram.h"
#include "ui_painter_histogram.h"
#include "global.h"

int painter_histogram::spectrum_data[SPECTRUM_PAINTER_WIDTH] = {0};
painter_histogram::painter_histogram(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::painter_histogram)
{
  ui->setupUi(this);
  //spectrum_data == NULL;
  different_display_color = 0;

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
//  connect(ui->b_back, SIGNAL(clicked()), this, SLOT()
}

painter_histogram::~painter_histogram()
{
  delete ui;
}


//#define rect_width 10
#ifdef FRIENDLYARM_TINY210
#define DISPLAY_ONT_SIZE 2
#endif
#ifdef FORLIN_OK335XS
#define DISPLAY_ONT_SIZE 5
#endif
void painter_histogram::paintEvent(QPaintEvent *)
{
  PRINT_DEBUG_INFOR;
  if(spectrum_data == NULL)return;
    int rect_width = this->width()/130;
    int side = qMin(width(), height());                                           //创建窗口宽高参数
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);                //开启抗锯齿
    painter.translate(20 * DISPLAY_ONT_SIZE, height() -50);                               //坐标系统平移变换，把原点平移到SUIT POSITION
    painter.scale(side / 300.0, side / 300.0);            //坐标系统比例变换，使绘制的图形随窗口的放大而放大
    //painter.scale(1, -1);                                          //Y轴向上翻转，翻转成正常平面直角坐标系

    painter.setPen(QPen(Qt::black, height() /1000));
    painter.setBrush(Qt::cyan);
    painter.setFont(QFont("Times", DISPLAY_ONT_SIZE*2));

    painter.drawLine(0,0,2000,0);
    painter.drawLine(0,0,0,-2000);

    for(int i = 0; i < 9; i++){
        // y axes;
        painter.drawText(-6* DISPLAY_ONT_SIZE,- i * 25 ,QString::number(i * 500));
        painter.drawLine(0,- i * 25,5,- i * 25);
        //painter.drawRoundRect();
      }
    PRINT_DEBUG_INFOR;
    for(int i = 1;i <= SPECTRUM_PAINTER_WIDTH ;i++){
        int value = -(*(spectrum_data + i - 1))/20;
        painter.drawLine(i*rect_width - rect_width/2 ,0,i*rect_width - rect_width/2,5);//坐标上的line
        if((i-1) % 5 == 0)painter.drawText(i*rect_width  - DISPLAY_ONT_SIZE * 2,10,QString::number((double)(i-1)/10,'f',1));//坐标上的值

        if(i == different_display_color){
            painter.setBrush(Qt::blue);
            painter.drawRect((i-1)*rect_width,0\
                             ,rect_width,value);
            painter.setBrush(Qt::cyan);
            continue;
          }
        painter.drawRect((i-1)*rect_width,0\
                         ,rect_width,value);
        //painter.drawText((i-1)*rect_width + DISPLAY_ONT_SIZE/2 - 1,value - 4 ,QString::number(*(spectrum_data + i - 1)));//柱状图上的值
        //painter.drawLine(i*rect_width - DISPLAY_ONT_SIZE,3 * i * (i - 15),(i + 1)*rect_width - DISPLAY_ONT_SIZE,   3 * (i+1) * ((i+1) - 15));//throw line
    }
   //spectrum_data = (int [48])0;
    PRINT_DEBUG_INFOR;
}

void painter_histogram::on_pushButton_clicked()
{
    this->close();
}

void painter_histogram::spectrum_show_and_update(int summit_value)
{
  different_display_color = summit_value -1;
  on_b_forword_clicked();
  this->showFullScreen();
}
//void painter_histogram::spectrum_show_and_update()
//{
//  //different_display_color =0;
//  this->showFullScreen();
//}

void painter_histogram::on_b_forword_clicked()
{
  if(different_display_color >= SPECTRUM_PAINTER_WIDTH - 1)
    {
      different_display_color = SPECTRUM_PAINTER_WIDTH - 1;
      return;
    }
  different_display_color ++;
  ui->label->setText(QString("当前阈值:%1V 计数值:%2").arg(QString::number((double)(different_display_color )/10,'f',1))\
                     .arg(*(spectrum_data + different_display_color)));
  this->update();
}

void painter_histogram::on_b_back_clicked()
{
  PRINT_DEBUG_INFOR;
  qDebug() << different_display_color;
  if(different_display_color <= 0)
    {
      different_display_color = 0;
      return;
    }
  different_display_color --;
    ui->label->setText(QString("当前阈值:%1V 计数值:%2").arg(QString::number((double)(different_display_color)/10,'f',1))\
                       .arg(*(spectrum_data + different_display_color)));
    this->update();
    PRINT_DEBUG_INFOR;
}

void painter_histogram::just_updae(){
  this->update();
}
