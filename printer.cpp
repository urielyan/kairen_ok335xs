#include <QStringList>
#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include <qsqlquery.h>
#include <database.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "printer.h"
#include "global.h"
#include "wininforlistdialog.h"

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
    //fd = open( "/dev/pts/7", O_RDWR|O_NOCTTY|O_NDELAY);
  //fd = open( "/dev/ttySAC0", O_RDWR|O_NOCTTY|O_NDELAY);
  //fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
  if (-1 == fd){
      printf("open /dev/ttyO2 err\n");
      return;
    }
  if  ( tcgetattr( fd,&options)  !=  0){
      printf("SetupSerial tcgetattr\n");
      return;
    }
  cfsetispeed(&options,B9600);
  cfsetospeed(&options,B9600);

//  cfsetispeed(&options,B115200);
//  cfsetospeed(&options,B115200);
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

  QTimer *m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(recvData()));
  m_timer->start(3000);

  printf("open /dev/ttyO2 success");
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
#ifdef TRANST_DATA_TO_PC
    return 0;
#endif
  if(write(fd,&c,1) <= 0){
      printf("write err \n");
      return -1;
    }
  return 1;
}
int printer::transmit(void *data,int size){
#ifdef TRANST_DATA_TO_PC
    return 0;
#endif
    if(sizeof(data) <= 0) return 0;
  int ret  = write(fd,data,size);

  if(ret <= 0){
      printf("write err \n");
      return -1;
    }
  return ret;
}

int printer::transmit(long long data, int size){
#ifdef TRANST_DATA_TO_PC
    return 0;
#endif
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
#ifdef TRANST_DATA_TO_PC
    return 0;
#endif
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
#ifdef TRANST_DATA_TO_PC
    return 0;
#endif
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

#include <QVariant>
QByteArray printer::getSampleData()
{
    QByteArray out2;

    //out2.append(QString::number(0xff, 16));
    QSqlQuery query(Database::instance()->getDb());
    bool ok = query.exec("SELECT * FROM sample_data");
    if(ok == true )
    {
        while(query.next())
        {
            QString msgstr;
            for(int i = 0;i < 10;i++)
            {
                if(i == Database::IsAuto || i == Database::CurrentCoefficient)
                {
                    continue;
                }
                msgstr += Database::getDefaultData(i, query.value(i).toString());
                msgstr += "|";
            }
            msgstr.chop(1);
            out2.append(msgstr).append("\n");
        }
    }
    //out2.append(QString::number(0xfe, 16));
    out2.chop(1);

    return out2;
}

#include <QDebug>
QString getStringId(uint num)
{
    QString id = "000";
    id.chop(QString::number(num).size());
    return id.append(QString::number(num));
}

QByteArray printer::revertDataToOtherProtectet(QByteArray rawData)
{
    QString sendData;
    uint id = 0;
    QStringList rawDatas = QString(rawData).split("\n");
    foreach (QString data, rawDatas)
    {
        QStringList datas = data.split("|");
        sendData.append(getStringId(id++));
        sendData.append(datas.value(Database::DateTime));
        sendData.append(datas.value(Database::WorkCurve));
        sendData.append(datas.value(Database::MeasurementTime));
        sendData.append(datas.value(Database::RepeatTime));
        sendData.append(datas.value(Database::Average));
        sendData.append(datas.value(Database::Deviation));
        sendData.append(datas.value(Database::PeopelName));
        sendData.append(datas.value(Database::SampleSerial));
    }
    return sendData.toLocal8Bit();
}

void printer::recvData()
{
    printf("com0\n");
    write(fd, "test\n", 5);

    char buf[1] = {0};
    if(read(fd, buf, 1) != 1)
    {
        printf("read data err\n" );
        return;
    }

    if(buf[0] != 0x01)
    {
        printf( "data[1] != 0x01\n");
        return;
    }

    QByteArray out2 = getSampleData();
    out2 = revertDataToOtherProtectet(out2);

    printf("write data size %d!\n", out2.size());

    int sendSizeSum = 0;
    while (out2.size() > 0)
    {
        QByteArray tmpByteArray = out2.left(1024);
        sendSizeSum += write(fd, tmpByteArray.data(), tmpByteArray.size());
        out2.remove(0, tmpByteArray.size());
        printf("write data size %d!\n", out2.size());
        //QMessageBox::warning(NULL, "test", QString::number(sendSizeSum));

    }
    //QMessageBox::warning(NULL, "testend", QString::number(sendSizeSum));

    if(sendSizeSum == out2.size())
    {

        printf("数据已发送至ＰＣ!\n");
    }else
    {
        printf("数据发送到ＰＣ不成功!\n");
    }
}
