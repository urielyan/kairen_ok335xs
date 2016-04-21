#include "buzzer.h"
#include <stdio.h>
#include <stdlib.h>	//system
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ispreheat.h"

int hig[]= {14000, 0,0,1050,15000,15068,1760,1976,2000,3000,4000,14000};
#define ALREADY_START 0
#define NO_START 1
buzzer::buzzer(QObject *parent) :
  QObject(parent)
{
  int pd;
  char buf[32];
  pd = open("/proc/boardname", O_RDONLY);
  if (read(pd, buf, 32) < 0){
      perror("read error");
    }
  if (strncmp(buf,"OK335xD",7) == 0) {
      perror("no buzzer");
    }
  ::close(pd);


  fd = ::open("/dev/buzzer", O_RDWR);
  //printf("fd = %d\n",fd);
  if (fd < 0) {
      perror("open buzzer device");
    }

  i = 0;
  QTimer *timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(play_music()));
  timer->start(1000);
}

//buzzer::~buzzer(){
//  close(fd);
//}


void buzzer::start_music(){
  if(already_start_flag == ALREADY_START){
      return;
    }
  already_start_flag = ALREADY_START;
  //printf("already start music\n");
  //timer->start(10);
}

void buzzer::stop_music(){
//  if(already_start_flag == NO_START){
//      return;
//    }
  ioctl(fd, 0,100);
  ioctl(fd, 1, 0);
  already_start_flag = NO_START;
  //timer->stop();
  //printf("already stop music\n");
}

void buzzer::play_music(){
  if(ispreheat::is_sampling_num != NEED_START_SAMPLING){
      ioctl(fd, 0,100);
      ioctl(fd, 1, 0);
      already_start_flag = NO_START;
      //timer->stop();
      //printf("already stop music\n");
      return;
    }
  //printf("is playing music%d\n",i);
  if(i >= 3) i = 0;
      ioctl(fd, 1, hig[i]);
//      //usleep(3200000/length[i]);
//    }
  i++;
}
