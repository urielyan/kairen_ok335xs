#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>

#define SEGMENT_LENGTH        30

#define TRANST_DATA_TO_PC
class printer : public QObject
{
  Q_OBJECT
public:
  static printer *instance();
  explicit printer(QObject *parent = 0);
  ~printer();

  static void printCurrentDateTime();
  static void printStart();
  static void printEnd();
  static void printEnter();

  static int fd;
  static struct termios options;
  static int transmit(char);
  static int transmit(void *,int);
  static int transmit(long long ,int);
  static int transmit(unsigned long ,int);
  static int transmit(int ,int);

public slots:

  //void initData();
  static QByteArray getSampleData();
  static QByteArray revertDataToOtherProtectet(QByteArray rawData);
  static void recvData();
};

#endif // PRINTER_H


