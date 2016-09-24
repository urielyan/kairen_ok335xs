#include <QStringList>
#include <QMessageBox>
#include <QString>
#include <QDateTime>

#include "printer.h"
#include "global.h"

static int enter = 0x0A;

printer* printer::instance()
{
  static printer Instance;
  return &Instance;
}

printer::printer(QObject *parent) :
  QObject(parent)
{
#if 1
  //printf("in...%s\n",__FUNCTION__);

  fd = open( "/dev/ttyO2", O_RDWR|O_NOCTTY|O_NDELAY);
  //fd = open( "/dev/ttySAC0", O_RDWR|O_NOCTTY|O_NDELAY);
  //fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
  if (-1 == fd){
      return;
    }
  if  ( tcgetattr( fd,&options)  !=  0){
      printf("SetupSerial tcgetattr\n");
      return;
    }
  cfsetispeed(&options,B9600);
  cfsetospeed(&options,B9600);
  options.c_cflag |= CLOCAL | CREAD;//修改控制模式，保证程序不会占用串口和能够从串口中读取输入数据
  options.c_cflag &= ~CRTSCTS;//设置数据流控制
  options.c_cflag &= ~CSIZE;//屏蔽其他标志位
  options.c_cflag |= CS8;//设置8位数据位
  options.c_cflag &= ~PARENB;//设置无奇偶校验位
  options.c_iflag &= ~INPCK;//设置校验位
  options.c_cflag &= ~CSTOPB;//设置一位停止位
  options.c_oflag &= ~OPOST; //修改输出模式，原始数据输出
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_cc[VTIME] = 1;
  options.c_cc[VMIN] = 1;
  tcflush(fd,TCIFLUSH);
  if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
      printf("com set err,tcsetattr!\n");
      return;
    }
#endif

  //initData();
}
int printer::fd;
struct termios printer::options;

printer::~printer(){
  close(fd);
}

void printer::printCurrentDateTime()
{
  int date  = 0xDAC6D5C8;
  int year = 0xEAC4;
  int month = 0xC2D4;
  int day  = 0xD5C8;
  printer::transmit(date,4);
  printer::transmit((void *)"   ",3);

  //年
  QString date_year = QString::number(QDate::currentDate().year());
  printer::transmit((void *)date_year.toLocal8Bit().data(),date_year.size());
  printer::transmit(year,2);

  //月
  QString date_month = QString::number(QDate::currentDate().month());
  if(date_month.size() == 1)
    {
      date_month = "0" +date_month;
    }
  printer::transmit((void *)date_month.toLocal8Bit().data(),date_month.size());
  printer::transmit(month,2);

  //日
  QString date_day = QString::number(QDate::currentDate().day());
  if(date_day.size() == 1)
    {
      date_day = "0" +date_day;
    }
  printer::transmit((void *)date_day.toLocal8Bit().data(),date_day.size());
  printer::transmit(day,2);

  //3 blank
  printer::transmit((void *)"   ",3);

  //当前时间
  QString stringHour = QString::number(QTime::currentTime().hour());
  if(stringHour.size() == 1)
    {
      stringHour = "0" + stringHour;
    }
  printer::transmit((void *)stringHour.toLocal8Bit().data(),2);

  printer ::transmit((void *)":",1);

  QString stringMinute = QString::number(QTime::currentTime().minute());
  if(stringMinute.size() == 1)
    {
      stringMinute = "0" + stringMinute;
    }
  printer::transmit((void *)stringMinute.toLocal8Bit().data(),2);
  printer ::transmit(enter,1);
}

void printer::printStart()
{
  printer ::transmit(enter,1);
  printer::transmit((void *)"========================================",SEGMENT_LENGTH);
  printer ::transmit(enter,1);
}

void printer::printEnd()
{
//    printer ::transmit(enter,1);
//    printer::transmit((void *)"   ",3);
    printer::transmit((void *)"========================================",SEGMENT_LENGTH);
    printer ::transmit(enter,1);
    printer::transmit((void *)"   ",3);
    printer ::transmit(enter,1);
}

void printer::printEnter()
{
    printer ::transmit(enter,1);
}

int printer::transmit(char c){
  if(write(fd,&c,1) <= 0){
      printf("write err \n");
      return -1;
    }
  return 1;
}
int printer::transmit(void *data,int size){
  if(sizeof(data) <= 0) return 0;
  int ret  = write(fd,data,size);

  if(ret <= 0){
      printf("write err \n");
      return -1;
    }
  return ret;
}

int printer::transmit(long long data, int size){
  if (0 == data || size <= 0){
      return 0;
    }
  int ret  = write(fd,(void *)&data,size);

  if(ret <= 0){
      printf("write err \n");
      return -1;
    }
  return ret;
}

int printer::transmit(unsigned long data, int size){
  if (0 == data || size <= 0){
      return 0;
    }
  int ret  = write(fd,(void *)&data,size);

  if(ret <= 0){
      printf("write err \n");
      return -1;
    }
  return ret;
}

int printer::transmit(int data, int size){
  if (0 == data || size <= 0){
      return 0;
    }
  int ret  = write(fd,(void *)&data,size);

  if(ret <= 0){
      printf("write err \n");

      return -1;
    }
  return ret;
}
