
#include "ispreheat.h"
#include "ui_ispreheat.h"
#include <QTimer>
#include <QSettings>

#include "widget.h"
#include "issample.h"

extern int measurement_flag;
#define PWM_IOCTL_SET_FREQ	1
#define PWM_IOCTL_STOP		0

int ispreheat::fd = 0;
ispreheat::ispreheat(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ispreheat)
{
  ui->setupUi(this);

  ui->label->setText("预热还需30分钟");
  ui->label_2->hide();
  global_ispreheat = 30;
  blink_flag = 1;
  timer = new QTimer(this);
  timer2 = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(check_ispreheat()));
  timer->start(1000 * 60);//

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
    }
}
int ispreheat::is_sampling_num = 0;

ispreheat::~ispreheat()
{
  ::close(fd);
  delete ui;
}

void ispreheat::check_ispreheat(){
  global_ispreheat -=1;
  QString tmpstr =  QString("预热还需%1分钟").arg(global_ispreheat);
  ui->label->setText(tmpstr);
  if(global_ispreheat <= 0){
      ui->label->hide();
      timer->stop();

      ui->label_2->show();
      connect(timer2,SIGNAL(timeout()),this,SLOT(change_label_content()));
      timer2->start(1000);
    }
}
void ispreheat::change_label_content(QString str){
  ui->label_2->setText(str);
}
void ispreheat::change_label_content(){
  //static bool blink_flag = 1;

  //ui->label_2->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));

  if((is_sampling_num >= 0 && is_sampling_num <= COUNT_MEASUREMENT_DATA_COUNT) || is_sampling_num == NEED_START_SAMPLING){
      if(1 == blink_flag){
          //int ret = 0;
          switch (is_sampling_num) {
            case NEED_START_SAMPLING:
              ui->label_2->setText("请移动滑板到参考样采样!");
              blink_flag  = 0;
              //buzzer::start_music();
              break;
            case 0:
              //ui->label_2->setText("");
              ui->label_2->setStyleSheet("background-image:url(/sdcard/0.png);background-repeat:no-repeat");
              blink_flag = 0;
              //buzzer::stop_music();
              break;
            case 1:
              ui->label_2->setText("正在采样①");
              //ui->label_2->setStyleSheet("background-image:url(/sdcard/1.png);background-repeat:no-repeat;background-position:top left");
              blink_flag = 0;
              break;
            case 2:
              ui->label_2->setText("正在采样①②");
              //ui->label_2->setStyleSheet("background-image:url(/sdcard/2.png);background-repeat:no-repeat;background-position:top left");
              blink_flag = 0;
              break;
            case 3:
              ui->label_2->setText("正在采样①②③");
              //ui->label_2->setStyleSheet("background-image:url(/sdcard/3.measurement_flagmeasurement_flagpng);background-repeat:no-repeat;background-position:top left");
              blink_flag = 0;
              break;
            case 4:
              ui->label_2->setText("正在采样①②③④");
              //ui->label_2->setStyleSheet("background-image:url(/sdcard/4.png);background-repeat:no-repeat;background-position:top left");
              blink_flag = 0;
              break;
            case 5:
              ui->label_2->setText("正在采样①②③④⑤");
              blink_flag = 0;
              break;
            case 6:
              ui->label_2->setText("正在采样①②③④⑤⑥");
              blink_flag = 0;
              break;
            case 7:
              ui->label_2->setText("正在采样①②③④⑤⑥⑦");
              blink_flag = 0;
              break;
            case 8:
              ui->label_2->setText("正在采样①②③④⑤⑥⑦⑧");
              blink_flag = 0;
              break;
            case 9:
              ui->label_2->setText("正在采样①②③④⑤⑥⑦⑧⑨");
              blink_flag = 0;
              break;
            case 10:
              ui->label_2->setText("正在采样①②③④⑤⑥⑦⑧⑨⑩");
              blink_flag = 0;
              break;
            default:
              blink_flag = 0;
              break;
            }
        }else{
//          ioctl(fd, PWM_IOCTL_STOP);
//          int ret = ioctl(fd, PWM_IOCTL_SET_FREQ, 0);
//          if(ret < 0){
//              perror("set the frequency of the buzzer2");
//              //exit(1);
//            }
          ui->label_2->setText("");
          blink_flag = 1;
        }
    }
#ifdef DEBUG
  ui->label_2->setText(ui->label->text().append(QString::number(measurement_flag)));
#endif
}
