#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

class printer : public QObject
{
  Q_OBJECT
public:
  static printer *instance();
  explicit printer(QObject *parent = 0);
  ~printer();

  static void printCurrentDateTime();

  static int fd;
  static struct termios options;
  static int transmit(char);
  static int transmit(void *,int);
  static int transmit(long long ,int);
  static int transmit(unsigned long ,int);
  static int transmit(int ,int);
};

#endif // PRINTER_H


