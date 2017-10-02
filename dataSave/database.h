#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFrame>
#include <QSqlDatabase>

class Database : public QFrame
{
  Q_OBJECT

public:
    enum ColumnName
    {
        PeopelName = 0,
        SampleSerial,
        DateTime,
        WorkCurve,
        MeasurementTime,
        RepeatTime,
        Average,
        Deviation,
        IsAuto,
        CurrentCoefficient
    };

    static Database* instance();

  QSqlDatabase getDb() const;
  static QString getDefaultData(uint column, QString rawData);

signals:

public slots:

private:
  explicit Database(QFrame *parent = 0);
  QSqlDatabase db;
};

#endif // DATABASE_H
