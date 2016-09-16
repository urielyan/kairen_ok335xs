#ifndef DATASAVE_H
#define DATASAVE_H
#include <QApplication>
#include <QDebug>
#include <QSettings>

class DataSave
{
public:
  static DataSave *instance();
  void test();
  QVariant value(QString key);
  void setValue(QString key, qint64 value);

private:
  DataSave();
  QSettings *settings;
};

#endif // DATASAVE_H
