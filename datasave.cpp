#include "datasave.h"
#include "global.h"
DataSave *DataSave::instance()
{
  static DataSave _instance;
  return &_instance;
}

void DataSave::test()
{
  setValue("test", 1);
  qDebug() <<  settings->value("test") << settings->fileName();
  if(settings->value("test").toInt() != 1)
    {
      PRINT_DEBUG_INFOR;
      qDebug() << "QSettings err!";
  }
}

QVariant DataSave::value(QString key)
{
  return settings->value(key);
}

void DataSave::setValue(QString key, qint64 value)
{
  settings->setValue(key, value);
}

DataSave::DataSave()
{
  settings = new QSettings("./QSettings/test.settings", QSettings::NativeFormat);

  //  QCoreApplication::setOrganizationName("shanghaikairen");
  //  QCoreApplication::setApplicationName("analysis");
}
