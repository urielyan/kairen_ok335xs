#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QApplication>
#include  <QDesktopWidget>

#include "wininforlistdialog.h"

#define SLIDING_PLATE_CHANGE_TIME   10
#define REFERENCE_BE_LOCATON        1
#define WAIT_BE_LOCATION            2
#define MACHINE_MALFUNCTION         3
#define SLIDING_PLATE_NO_CHANGE     4
#define REFERENCE_BE_LOCATON_TEXT        "参考样处在测量位置"
#define WAIT_BE_LOCATION_TEXT            "待测样处在测量位置"
#define MACHINE_MALFUNCTION_TEXT         "机械故障，滑板没到位，请检查"
#define SLIDING_PLATE_NO_CHANGE_TEXT     "没有接受到滑板改变信息，请查看机械"
#define TRANSMIT_DATA_ERROR     "通讯故障"
#define SLIDING_PLATE_NOT_ALLOW "您移动滑板过于频繁, 请等待几秒"

#define CALIBRATE_RESULT_MAX_RECORD 20
#define NO_BEHAVIOUR_START_COUNT       10 //* 6
#define MESSAGEBOX_AUTO_CLOSE_SECOND    3
#define TURN_SECONDS 31//count a measurement time

#define MEASUREMENT_NOTHING     0
#define MEASUREMENT_SPECTRUM    1
#define MEASUREMENT_COUNT       2
#define MEASUREMENT_CALIBRATE   3
#define MEASUREMENT_SAMPLE      4
#define MEASUREMENT_10_AUTO     5
#define MEASUREMENT_LOOP_COUNT  6
#define MEASUREMENT_STEADY_SUMMIT 7

#define SETUP_WAIT_TIME         2

#define SUCCESS_SPECTOSCOPE     1111
#define DEFEAT_SPECTOSCOPE        1000
#define SUCCESS_COUNT           2222
#define DEFEAT_COUNT              2000

#define COUNT_MEASUREMENT_MOST_STORAGE 10000

#define COUNT_MEASUREMENT_DATA_COUNT 7

#define FONT_SIZE 24
#define FONT_NAME "wenquanyi"

#define ERRNO_COMMUNICATION_1  1
#define ERRNO_SILIDING_POSITION   2
#define ERRNO_DATA_ILLEGAL             3
#define ALL_RIGHT 0

#define DESKTOP_WIDTH   QApplication::desktop()->width()
#define DESKTOP_HEIGHT QApplication::desktop()->height()

//#define WIDGET_FONT_SIZE 30;
//#define DEBUG

//class WinInfoListDialog : public QDialog
//{
//    Q_OBJECT
//public:
//    static WinInfoListDialog *instance();

//    void showMsg(quint32 ErrorNum = 0);

//    static QString getStrMsgbyNum(quint32 ErrorNum);
//signals:

//public slots:
//    void slotCloseClicked();

//private:
//    WinInfoListDialog(QWidget *parent = 0);
//    ~WinInfoListDialog();

//private:
//    QFrame *p_mainFrame;
//    QLabel *pictureLabel;   //标题栏图片
//    QLabel *titleLabel;     //标题栏文字
//    QLabel *txtLabel;       //错误信息
//    quint32 m_ErrorNum;     //错误代码号
//};
#endif // GLOBAL_H
