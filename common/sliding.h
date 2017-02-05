
#ifndef SLIDING_H
#define SLIDING_H
#include <QObject>
#include "wininforlistdialog.h"

class Sliding : public QObject
{
    Q_OBJECT

public:
    enum SliderPosition
    {
        Reference = 1,          //参考样
        SampleToBeTested = 2,   //待测样
        Malfunction = 3         //机械故障
    };

    enum SlidingDirection
    {
        In = 0,                 //移进滑板
        Out                     //移出滑板
    };

    enum SliderState
    {
        OutCount = 0,           //滑动滑板次数过多。
        TransmitErr,            //发送移动滑板命令错误。
        ReceiveNull,            //未接收到数据。
        UnKnowErr,              //不知道接收的数据，打印当前的数据。
        SlidingSuccess          //滑板到位

    };

    explicit Sliding(QObject *parent = 0);

    static int moveSlider(SlidingDirection direction);

    static void insertSliderButtonPairs(QPushButton* in, QPushButton* Out);
    static void setButtonsState(SliderPosition position);

    static QList<QPushButton*> s_moveInSliderButtons;
    static QList<QPushButton*> s_moveOutSliderButtons;

private:

};
#endif // SLIDING_H
