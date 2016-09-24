#ifndef DATASAVE_H
#define DATASAVE_H
#include <QApplication>
#include <QDebug>
#include <QSettings>

class MeasurementDataSave
{

public:
  static MeasurementDataSave *instance();

public:
  bool test();
  QVariant value(QString key);
  void setValue(QString key, qint64 value);
  void setValue(QString key, QString value);

protected:
  //virtual void init() = 0;
  QSettings *settings;

private:
  MeasurementDataSave();
  void init();
};

class ErrorCountSave
{
public:
  static ErrorCountSave *instance();

  void addCount(uint which);

public:
  bool test();
  QVariant value(QString key);
  void setValue(QString key, qint64 value);
  void setValue(QString key, QString value);

protected:
  //virtual void init() = 0;
  QSettings *settings;


private:
  ErrorCountSave();
  void init();
};
#endif // DATASAVE_H
