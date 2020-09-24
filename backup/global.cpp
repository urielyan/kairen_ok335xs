#include "global.h"
//WinInfoListDialog *WinInfoListDialog::instance()
//{
//    static WinInfoListDialog data;
//    return &data;
//}


GlobalData *GlobalData::instance()
{
  static GlobalData g;
  return & g;
}

GlobalData::GlobalData(QObject *parent) : QObject(parent)
{
  strComName = QString("");
  strUdiskPath = QString("");
}
