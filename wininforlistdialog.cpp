#include "wininforlistdialog.h"

WinInforListDialog *WinInforListDialog::instance()
{
  static WinInforListDialog Instance;
  return &Instance;
}

void WinInforListDialog::showMsg(qint32 id)
{
  switch (id) {
    case 0:
      break;
    default:
      break;
    }

  this->show();
}

WinInforListDialog::WinInforListDialog(QWidget *parent) : QDialog(parent)
{
  setStylesheet();
}

void WinInforListDialog::setStylesheet()
{

}

