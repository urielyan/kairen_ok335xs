#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFrame>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlError>
#include <QSqlQuery>

class Database : public QFrame
{
  Q_OBJECT
public:
  static Database* instance();

  QSqlDatabase getDb() const;

signals:

public slots:

private:
  explicit Database(QFrame *parent = 0);
  QSqlDatabase db;
};

#endif // DATABASE_H
