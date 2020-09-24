#include "settings_datasave.h"

Settings_DataSave *Settings_DataSave::instance()
{
  static Settings_DataSave tmp;
  return &tmp;
}

Settings_DataSave::Settings_DataSave(QObject *parent) :
  QObject(parent)
{

}
