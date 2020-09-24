#include <QDateTime>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "database.h"

Database *Database::instance()
{
  static Database _instance;
  return &_instance;
}

Database::Database(QFrame *parent) :
  QFrame(parent)
{
  db= QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("./samplemeasurement.db");
  bool ok = db.open();
  if(ok == false){
      QMessageBox::warning(this,QString("db open err"),db.lastError().text());
      return;
    }
  QStringList tableList = db.tables();
  if(!tableList.contains("sample_data"))
    {
      QSqlQuery query(db);
      ok = query.exec("CREATE TABLE sample_data(people_id,sample_serial,date_time,work_curve,measurement_time,repeat_time,average,deviation,is_auto,current_coefficient);");
      if(ok == false){
          QMessageBox::warning(this,"create table err",query.lastError().text());
          return;
        }
    }
}
QSqlDatabase Database::getDb() const
{
    return db;
}

QString Database::getDefaultData(uint column, QString rawData)
{
    QString sendData = rawData;
    switch (column)
    {
    case PeopelName:
    case SampleSerial:
        if(rawData.size() >= 5)
        {
            sendData = rawData.right(5);
        }else
        {
            sendData = QString("99999");
        }
        break;

    case DateTime:
    {
        sendData = rawData.remove("-").remove(" ");
        if(sendData.size() != 13)
        {
            sendData = "1999090909:09";
        }
        break;
    }
    case WorkCurve:
        if (rawData.size() == 0)
        {
            sendData = "00";
        } else if (rawData.size() == 1)
        {
            sendData = "0" + rawData;
        } else
        {
            sendData = "99";
        }
        break;

    case MeasurementTime:
        if (rawData.size() == 2)
        {
            sendData = "0" + rawData;
        }
        else if (rawData.size() != 3)
        {
            sendData = "999";
        }
        break;
    case RepeatTime:
        if (rawData.size() == 1)
        {
            sendData = "0" + rawData;
        }
        else if(rawData.size() != 2)
        {
            sendData = "99";
        }
        break;
    case Average:
        if(rawData.size() != 6)
        {
            sendData = "9.9999";
        }else
        {
            sendData = rawData;
        }
        break;
    case Deviation:
        if(rawData.size() != 6)
        {
            sendData = "9.9999";
        }else
        {
            sendData = rawData;
        }
        break;
    default:
        break;
    }

    return sendData;
}
