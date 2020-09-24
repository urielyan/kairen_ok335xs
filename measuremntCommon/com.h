
#ifndef COM_H
#define COM_H
#include <termios.h>
#include <QObject>
#include <QTimer>
//本pc机采用的是小端架构的，而arm的机械一般是大端的，所以所有的命令都是倒序的
#define STOP_ORDER              0xff98feUL
#define CURRENT_POSITION        0XFF98FEUL
#define ACTIVATING_SPECTRUM     0XFF01FEUL
#define ACTIVATINE_COUNT        0XFF02FEUL
//#define ACTIVATING_CONTENT      0XFF03FEUL
#define ACTIVATING_CALIBRATE    0XFF04FEUL
#define GET_VERSION   0XFF05FEUL
#define IN_SLIDING_PLATE        0XFF3106FEUL
#define OUT_SLIDING_PLATE       0XFF3206FEUL

class SendSampleDataToPC : public QObject
{
      Q_OBJECT
public:
  explicit SendSampleDataToPC(QObject *parent = 0);

public slots:
    static void sendData(QString data);
    static void recvData();
    static QString receive(int fd);

    static int openCom();
    static QByteArray getSampleData();

private:
};

class Communciation_Com : public QObject
{
  Q_OBJECT
public:
    enum Move_Plate_Direction
    {
        MoveToReference = 0,
        MoveToMeasuring
    };

      static Communciation_Com* instance();

  static int fd;
  static struct termios options;
  static int transmit(char);
  static int transmit(void *,int);
  static int transmit(long long ,int);
  static int transmit(unsigned long ,int);
  static int transmit(int ,int);

  static QString receive();
  static QString receive(int);

  bool movePlate(Communciation_Com::Move_Plate_Direction direction);

public slots:

public:
  explicit Communciation_Com(QObject *parent = 0);
};

#endif // COM_H
