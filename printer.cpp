#include <QStringList>
#include <QMessageBox>

#include "printer.h"
#include "global.h"

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
}
int printer::fd;
struct termios printer::options;

printer::~printer(){
  close(fd);
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
        //QMessageBox::warning(NULL,"int","write");
        printf("write err \n");

        return -1;
    }
    return ret;
}
