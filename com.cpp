#include "com.h"
#include <QDebug>
#include "issample.h"
#include "global.h"

Communciation_Com::Communciation_Com(QObject *parent) :
    QObject(parent)
{
   // fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
#ifdef FORLIN_OK335XS
    fd = open( "/dev/ttyO1", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FRIENDLYARM_TINY210
        fd = open( "/dev/ttySAC3", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
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
}


Communciation_Com *Communciation_Com::instance()
{
  static Communciation_Com Instance;
  return &Instance;
}

int Communciation_Com::fd;
struct termios Communciation_Com::options;

int Communciation_Com::transmit(char c){
  if(write(fd,&c,1) <= 0){
      printf("write err\n");
      return -1;
    }
  return 1;
}
int Communciation_Com::transmit(void *data,int size){
    if(sizeof(data) <= 0) return 0;
    int ret  = write(fd,data,size);

    if(ret <= 0){
        printf("write err\n");
        return -1;
    }
    return ret;
}

int Communciation_Com::transmit(long long data, int size){
    if (0 == data || size <= 0){
        return 0;
    }
    int ret  = write(fd,(void *)&data,size);

    if(ret <= 0){
        printf("write err\n");
        return -1;
    }
    return ret;
}
int Communciation_Com::transmit(unsigned long data, int size){
    if (0 == data || size <= 0){
        return 0;
    }
    int ret  = write(fd,(void *)&data,size);

    if(ret <= 0){
        printf("write err\n");
        return -1;
    }
    return ret;
}
int Communciation_Com::transmit(int data, int size){
    if (0 == data || size <= 0){
        return 0;
    }
    int ret  = write(fd,(void *)&data,size);

    if(ret <= 0){
        printf("write err\n");
        return -1;
    }
    return ret;
}


QString Communciation_Com::receive(){
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 100;
    int ret = select(fd + 1,&fds, NULL, NULL , &time);
    if(0 == ret){
        return NULL;
    }else if(-1 == ret){
        return NULL;
    }
    char buf[20] = {0,};
    char b = 0;
    //err_count represent the read err number.
    int i = 0,start_flag = 0,err_count= 0;
    while(1){
        if(i > 17 || err_count >1000) break;
        if(read(fd , &b , 1) > 0){
            if(b == (char)0xfe){
                start_flag = 1;
                i = 0;
            }else if(b == (char)0xff){
                break;
            }else if(start_flag == 1){
                buf[i] = b;
                i++;
            }
        }else {
            usleep(10);
            err_count++;
            continue;
        }
    err_count++;
    }
    QString recv_data(buf);
    return recv_data;
}
QString Communciation_Com::receive(int wait_time){
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    struct timeval time;
    time.tv_sec = wait_time;
    time.tv_usec = 100;
    int ret = select(fd + 1,&fds, NULL, NULL , &time);
    if(0 == ret){
        return NULL;
    }else if(-1 == ret){
        return NULL;
    }
    char buf[20] = {0,};
    char b = 0;
    //err_count represent the read err number.
    int i = 0,start_flag = 0,err_count= 0;
    while(1){
        if(i > 17 || err_count >1000) break;
        if(read(fd , &b , 1) > 0){
            if(b == (char)0xfe){
                start_flag = 1;
                i = 0;
            }else if(b == (char)0xff){
                break;
            }else if(start_flag == 1){
                buf[i] = b;
                i++;
            }
        }else {
            usleep(10);
            err_count++;
            continue;
        }
    err_count++;
    }
    QString recv_data(buf);
    return recv_data;
}
