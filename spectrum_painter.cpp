#include "spectrum_painter.h"
#include "ui_spectrum_painter.h"

#include <QWidget>
#include <QPainter>
#include <QDebug>

spectrum_painter::spectrum_painter(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::spectrum_painter)
{
  ui->setupUi(this);
  v1 = v2 = v3 = 0;//v1==k;v2==b;v3==r/v1==a1;v2==a2;v3=a3;
}

spectrum_painter::~spectrum_painter()
{
  delete ui;
}

void spectrum_painter::on_pushButton_clicked()
{
  this->close();
}


#define DISPLAY_FONT_SIZE 5
//#define multiple_size 100
#define POINT_SIZE 2
static QMap<int,QString>painter_data;//painter data
void spectrum_painter::paintEvent(QPaintEvent *){
  //int rect_width = this->width()/130;
  int side = qMin(width(), height());                                           //创建窗口宽高参数
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing,true);                //开启抗锯齿
  painter.translate(25 * DISPLAY_FONT_SIZE, height() -50);//坐标系统平移变换，把原点平移到SUIT POSITION
  painter.scale(side / 300.0, side / 300.0);//坐标系统比例变换，使绘制的图形随窗口的放大而放大
  //painter.scale(1, -1);//Y轴向上翻转，翻转成正常平面直角坐标系

  painter.setPen(QPen(Qt::black, height() /1000));
  painter.setBrush(Qt::cyan);
  painter.setFont(QFont("Times", DISPLAY_FONT_SIZE*2));

  painter.drawLine(0,0,2000,0);
  painter.drawLine(0,0,0,-2000);
  //  v1= 1.5773;
  //  v2 = - 0.2151;

  max_data_y = max_data_x = 0;
  QMap<int,QString>::iterator i;
  if(curve <= 5){
      for(i = painter_data.begin();i != painter_data.end();i++){
          QStringList painter_data_list = i.value().split("/");
          if(painter_data_list.size() != 3)return;
          if(painter_data_list[0].toDouble() > max_data_y)max_data_y = painter_data_list[0].toDouble();
          if(painter_data_list[1].toDouble()/painter_data_list[2].toDouble()> max_data_x)max_data_x = painter_data_list[1].toDouble()/painter_data_list[2].toDouble();
        }
      max_data_y *= 1.1;//所要花的直线的最大的y的值在多0.1，TO保证穿过最后一个点
      max_data_x *= 1.2;
      multiple_size = 200/max_data_y;//放大倍数，画图的最大位置始终在y为200 的位置
      for(i = painter_data.begin();i != painter_data.end();i++){
          QStringList painter_data_list = i.value().split("/");
          if(painter_data_list.size() != 3)return;
          //+v2/v1 * multiple_size 是向左平移
          painter.drawRoundRect(painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size+v2/v1 * multiple_size ,
              - painter_data_list[0].toDouble() * multiple_size - POINT_SIZE,    POINT_SIZE,POINT_SIZE,99,99);
//          painter.drawText(painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size+v2/v1 * multiple_size ,
//              - painter_data_list[0].toDouble() * multiple_size - POINT_SIZE,
//              QString("(%1,%2)").arg(painter_data_list[1].toDouble()/painter_data_list[2].toDouble()).arg(painter_data_list[0].toDouble()));
//          qDebug()  << painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size+v2/v1 * multiple_size ;
        }

      painter.drawLine(0,0,(max_data_y - v2)/v1 * multiple_size +v2/v1 * multiple_size,-max_data_y * multiple_size);

      //i先乘以 100 再除以100是为了使y轴上的数只有两个小数点,
      for(int i = 0; i <= max_data_y * 100; i+= max_data_y * 10){
          // y axes;
          painter.drawText(-8* DISPLAY_FONT_SIZE,- (double)i/100 * multiple_size,QString::number((double)i/100));
          painter.drawLine(0,- (double)i/100 * multiple_size,-5,- (double)i/100 * multiple_size);
        }
      for(double i = -v2/v1;i < max_data_x; i += (max_data_x/5)){
          painter.drawText(i * multiple_size+v2/v1 * multiple_size  - DISPLAY_FONT_SIZE * 2 ,15,QString::number(i,'f',1));
          painter.drawLine(i * multiple_size+v2/v1 * multiple_size ,0,i * multiple_size+v2/v1 * multiple_size ,5);
        }
    }else{
      multiple_size = 50;
      for(i = painter_data.begin();i != painter_data.end();i++){
          QStringList painter_data_list = i.value().split("/");
          if(painter_data_list.size() != 3)return;
          //+v2/v1 * multiple_size 是向左平移
          painter.drawRoundRect(painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size ,
              - painter_data_list[0].toDouble() * multiple_size - POINT_SIZE,    POINT_SIZE + 2,POINT_SIZE + 2,99,99);
//          painter.drawText(painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size ,
//              - painter_data_list[0].toDouble() * multiple_size - POINT_SIZE,
//              QString("(%1,%2)").arg(painter_data_list[1].toDouble()/painter_data_list[2].toDouble()).arg(painter_data_list[0].toDouble()));
          //qDebug()  << painter_data_list[1].toDouble()/painter_data_list[2].toDouble() * multiple_size+v2/v1 * multiple_size ;
        }

      painter.setBrush(Qt::black);
      for(double i = 0;i < 3.0; i += 0.01){
          painter.drawRoundRect(i * multiple_size,-(v3 * i * i + v2 * i + v1) * multiple_size,1,1,99,99);// 圆在最上方，说明屏幕分辨率是530+50/280+50
        }
      painter.setBrush(Qt::cyan);

      max_data_y = 5.5;
      max_data_x = 3.5;
      for(int i = 0; i <= max_data_y * 100; i+= max_data_y * 10){
          // y axes;
          painter.drawText(-8* DISPLAY_FONT_SIZE,- (double)i/100 * multiple_size,QString::number((double)i/100));
          painter.drawLine(0,- (double)i/100 * multiple_size,-5,- (double)i/100 * multiple_size);
        }
      for(double i = 0;i < max_data_x; i += (max_data_x/5)){
          painter.drawText(i * multiple_size - DISPLAY_FONT_SIZE * 2 ,15,QString::number(i,'f',1));
          painter.drawLine(i * multiple_size,0,i * multiple_size,5);
        }
    }

  for(int i = 0; i <= max_data_y * 100; i+= max_data_y * 10){
      // y axes;
      painter.drawText(-8* DISPLAY_FONT_SIZE,- (double)i/100 * multiple_size,QString::number((double)i/100));
      painter.drawLine(0,- (double)i/100 * multiple_size,-5,- (double)i/100 * multiple_size);
    }
}

void spectrum_painter::show_special_curve(int current_curve,QMap<int,QString>data,QString kbr_value){
  if(kbr_value.split(";").size() != 3)return;
  v1 = kbr_value.split(";")[0].split("=")[1].toDouble();
  v2 = kbr_value.split(";")[1].split("=")[1].toDouble();
  v3 = kbr_value.split(";")[2].split("=")[1].toDouble();

  if(data.size() < 3 || data.size() > 11)return;
  painter_data = data;

  if(current_curve < 1 || current_curve > 9)return;
  curve = current_curve;
  this->update();
  this->showFullScreen();
}
