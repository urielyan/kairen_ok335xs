#include <QDebug>
#include <QObject>
#include <QSettings>
#include <QSqlQuery>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "com.h"
//#include "issample.h"
#include "global.h"
#include "wininforlistdialog.h"
#include "database.h"

Communciation_Com::Communciation_Com(QObject *parent) :
    QObject(parent)
{
#ifdef COMPUTE_DESKTOP
    fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FORLIN_OK335XS
    //fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
    fd = open( "/dev/ttyO1", O_RDWR|O_NOCTTY|O_NDELAY);//mid com
#endif
#ifdef FRIENDLYARM_TINY210
        fd = open( "/dev/ttySAC3", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
    if (-1 == fd){
        fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);//mid com
        if(fd == -1)
        {
            //WinInforListDialog::instance()->showMsg(tr("open err"));
            return;
        }
        return;
    }
    if  ( tcgetattr( fd,&options)  !=  0)
    {
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
    printf("open com success!\n");
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
    int ret  = write(fd, data, size);

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
    int ret  = write(fd,(void *)&data, size);

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

bool Communciation_Com::movePlate(Communciation_Com::Move_Plate_Direction direction)
{
    if(direction == MoveToMeasuring)
    {

    }else if(direction == MoveToReference)
    {

    }


//    {
//      if(measurement_flag != MEASUREMENT_NOTHING){
//          emit transmit_stop_auto_count();
//        }
//      tcflush(Communciation_Com::fd,TCIOFLUSH);
//      measurement_flag = MEASUREMENT_NOTHING;
//      if(Communciation_Com::transmit(OUT_SLIDING_PLATE,4) < 0){
//          WinInforListDialog::instance()->showMsg(tr(TRANSMIT_DATA_ERROR));
//          return -1;
//        }
//      QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
//      //qDebug() <<recv_data.toLocal8Bit().data();
//      if(recv_data == NULL){
//          WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + "recv NULL");
//          ErrorCountSave::instance()->addCount(6);
//          return -1;
//        }else if(recv_data[1] == (char)0x31){
//          ui->widget->change_label_content(REFERENCE_BE_LOCATON);
//        }else if(recv_data[1] == (char)0x33){
//          WinInforListDialog::instance()->showMsg(tr(MACHINE_MALFUNCTION_TEXT) + "recv NULL");
//          set_sliding_disabled(false,false);
//          emit transmit_move_sliding(false,false);
//          return -1;
//        }else {
//          WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NO_CHANGE_TEXT) + ":"
//                                                  + recv_data + "," + QString::number(measurement_flag));
//          ErrorCountSave::instance()->addCount(6);
//          return -1;
//        }
//      set_sliding_disabled(true,false);
//      emit transmit_move_sliding(true,false);
//      if(last_move_sliding_datetime.secsTo(QDateTime::currentDateTime()) < 5){
//          move_sliding_count++;
//        }else{
//          move_sliding_count = 0;
//        }
//      last_move_sliding_datetime = QDateTime::currentDateTime();
//      return ALL_RIGHT;
//    }
}

SendSampleDataToPC::SendSampleDataToPC(QObject *parent)
    : QObject(parent)
{
    QTimer *m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(recvData()));
    m_timer->start(3000);
}

#include <QMessageBox>
void SendSampleDataToPC::sendData(QString data)
{
    int fd = -1;
    static struct termios options;

#ifdef COMPUTE_DESKTOP
    fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FORLIN_OK335XS
    fd = open( "/dev/ttyO2", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FRIENDLYARM_TINY210
    fd = open( "/dev/ttySAC3", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
    if (-1 == fd)
    {
        fd = open( "/dev/ttyO2", O_RDWR|O_NOCTTY|O_NDELAY);//mid com
        if(fd == -1)
        {
            //WinInforListDialog::instance()->showMsg(tr("open err"));
            WinInforListDialog::instance()->showMsg(tr("数据发送到ＰＣ不成功!"));
            return;
        }
    }
    if  ( tcgetattr( fd,&options)  !=  0)
    {
        printf("SetupSerial tcgetattr\n");
        WinInforListDialog::instance()->showMsg(tr("数据发送到ＰＣ不成功!"));
        return;
    }
    cfsetispeed(&options,B115200);
    cfsetospeed(&options,B115200);
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
        WinInforListDialog::instance()->showMsg(tr("数据发送到ＰＣ不成功!"));
        return;
    }
    printf("open com success!\n");

    int sendSize = write(fd, data.toLocal8Bit().data(), data.size());
    if(sendSize == data.size())
    {
        WinInforListDialog::instance()->showMsg(tr("数据已发送至ＰＣ!"));
    }else
    {
        WinInforListDialog::instance()->showMsg(tr("数据发送到ＰＣ不成功!"));
    }
    close(fd);
}

void SendSampleDataToPC::recvData()
{
    int fd  = openCom();

    char buf[3] = {0};
    if(read(fd, buf, 3) != 3)
    {
        static int i = 0;
        //QMessageBox::warning(NULL, "haha", QString::number(i) + buf);
        //sleep(1);
        return;
    }
    printf("read data %s!\n", buf);

    if(buf[1] != 0x01)
    {
        return;
    }

    QByteArray out2 = getSampleData();


    printf("write data %s!\n", buf);

    int sendSize = write(fd, out2.data(), out2.size());
    if(sendSize == out2.size())
    {
        WinInforListDialog::instance()->showMsg(tr("数据已发送至ＰＣ!"));
    }else
    {
        WinInforListDialog::instance()->showMsg(tr("数据发送到ＰＣ不成功!"));
    }

    close(fd);
}

QString SendSampleDataToPC::receive(int fd)
{
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

int SendSampleDataToPC::openCom()
{
    int fd = -1;
    static struct termios options;

#ifdef COMPUTE_DESKTOP
    fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FORLIN_OK335XS
    fd = open( "/dev/pts/25", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
#ifdef FRIENDLYARM_TINY210
    fd = open( "/dev/ttySAC3", O_RDWR|O_NOCTTY|O_NDELAY);
#endif
    if (-1 == fd)
    {
        fd = open( "/dev/ttyO2", O_RDWR|O_NOCTTY|O_NDELAY);//mid com
        if(fd == -1)
        {
            //WinInforListDialog::instance()->showMsg(tr("open err"));
            qDebug() << "open err" << __FILE__ << __LINE__;
            return -1;
        }
    }
    if  ( tcgetattr( fd,&options)  !=  0)
    {
        printf("SetupSerial tcgetattr\n");
        qDebug() << __FILE__ << __LINE__;
        return -2;
    }
    cfsetispeed(&options,B115200);
    cfsetospeed(&options,B115200);
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
        return -3;
    }
    printf("open com success!\n");

    return fd;
}

QByteArray SendSampleDataToPC::getSampleData()
{
    QByteArray out2;

    out2.append(QString::number(0xff, 16));
    QSqlQuery query(Database::instance()->getDb());
    bool ok = query.exec("SELECT * FROM sample_data");
    if(ok == true )
    {
        while(query.next())
        {
            QString msgstr;
            for(int i = 0;i < 10;i++)
            {
                if(query.value(i).toString() == NULL)
                {
                    msgstr += "0";
                    msgstr += "|";
                    continue;
                }
                msgstr += query.value(i).toString();
                msgstr += "|";
            }
            msgstr.chop(1);
            out2.append(msgstr).append("\n");
        }
    }
    out2.append(QString::number(0xfe, 16));

    return out2;
}
