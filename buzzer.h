#ifndef BUZZER_H
#define BUZZER_H

#include <QObject>

class buzzer : public QObject
{
  Q_OBJECT
public:
  explicit buzzer(QObject *parent = 0);
    void start_music();
    void stop_music();
signals:

public slots:
    void play_music();

private:
    int fd,i;
    int already_start_flag;
};

#endif // BUZZER_H
