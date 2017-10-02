#include <QHBoxLayout>
#include <QTimer>

#include <unistd.h>

#include "ispreheat.h"
#include "widget.h"
#include "issample.h"
#include "global.h"


extern int measurement_flag;
#define PWM_IOCTL_SET_FREQ	1
#define PWM_IOCTL_STOP		0

ispreheat::ispreheat(QWidget *parent) :
  QWidget(parent)
{
    QHBoxLayout *p_mainLayout = new QHBoxLayout(this);
    p_mainLayout->addWidget(&m_labelIsPreheat);
    p_mainLayout->addWidget(&m_labelIsSampling);

  m_labelIsPreheat.setText(tr("预热还需60分钟"));
  m_labelIsSampling.hide();

  global_ispreheat = 60;
  blink_flag = 1;
  timer = new QTimer(this);
  timer2 = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(check_ispreheat()));
  timer->start(1000 * 60);//

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE/3 * 2 ,QFont::Normal));
      labellist[i]->setMinimumHeight(FONT_SIZE + 2);
    }
}
int ispreheat::is_sampling_num = 0;

ispreheat::~ispreheat()
{
}

void ispreheat::check_ispreheat(){
  global_ispreheat -=1;
  QString tmpstr =  QString("预热还需%1分钟").arg(global_ispreheat);
  m_labelIsPreheat.setText(tmpstr);
  if(global_ispreheat <= 0){
      m_labelIsPreheat.hide();
      timer->stop();

      m_labelIsSampling.show();
      connect(timer2,SIGNAL(timeout()),this,SLOT(change_label_content()));
      timer2->start(1000);
    }
}
void ispreheat::change_label_content(QString str){
  m_labelIsSampling.setText(str);
}
void ispreheat::change_label_content(){
  if((is_sampling_num >= 0 && is_sampling_num <= COUNT_MEASUREMENT_DATA_COUNT) || is_sampling_num == NEED_START_SAMPLING){
      if(1 == blink_flag){
          switch (is_sampling_num) {
            case NEED_START_SAMPLING:
              m_labelIsSampling.setText("请移动滑板到参考样采样!");
              blink_flag  = 0;
              break;
            case 0:
              m_labelIsSampling.setStyleSheet("background-image:url(/sdcard/0.png);background-repeat:no-repeat");
              blink_flag = 0;
              break;
            case 1:
              m_labelIsSampling.setText("正在采样①");
              blink_flag = 0;
              break;
            case 2:
              m_labelIsSampling.setText("正在采样①②");
              blink_flag = 0;
              break;
            case 3:
              m_labelIsSampling.setText("正在采样①②③");
              blink_flag = 0;
              break;
            case 4:
              m_labelIsSampling.setText("正在采样①②③④");
              blink_flag = 0;
              break;
            case 5:
              m_labelIsSampling.setText("正在采样①②③④⑤");
              blink_flag = 0;
              break;
            case 6:
              m_labelIsSampling.setText("正在采样①②③④⑤⑥");
              blink_flag = 0;
              break;
            case 7:
              m_labelIsSampling.setText("正在采样①②③④⑤⑥⑦");
              blink_flag = 0;
              break;
            case 8:
              m_labelIsSampling.setText("正在采样①②③④⑤⑥⑦⑧");
              blink_flag = 0;
              break;
            case 9:
              m_labelIsSampling.setText("正在采样①②③④⑤⑥⑦⑧⑨");
              blink_flag = 0;
              break;
            case 10:
              m_labelIsSampling.setText("正在采样①②③④⑤⑥⑦⑧⑨⑩");
              blink_flag = 0;
              break;
            default:
              blink_flag = 0;
              break;
            }
        }else{
          m_labelIsSampling.setText("");
          blink_flag = 1;
        }
    }
}
