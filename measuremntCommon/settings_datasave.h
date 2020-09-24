#ifndef SETTINGS_DATASAVE_H
#define SETTINGS_DATASAVE_H

#include <QObject>
#include <QSettings>

class Settings_DataSave : public QObject
{
  Q_OBJECT
public:
  Settings_DataSave* instance();
  explicit Settings_DataSave(QObject *parent = 0);

signals:

public slots:

private:
  QSettings settings;
};

#endif // SETTINGS_DATASAVE_H
