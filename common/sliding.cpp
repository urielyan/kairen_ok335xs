
#include "global.h"

#include <QDateTime>
#include <QPushButton>

#include "sliding.h"
#include "wininforlistdialog.h"
#include "com.h"
#include "datasave.h"
#include "issample.h"

QList<QPushButton*>Sliding::s_moveInSliderButtons;
QList<QPushButton*>Sliding::s_moveOutSliderButtons;
Sliding::Sliding(QObject *parent)
    : QObject(parent)
{

}

static int s_slidingCount = 0;
static QDateTime s_newestSlidingDateTime = QDateTime::currentDateTime();
int Sliding::moveSlider(SlidingDirection direction)
{
    //若连续滑动三次，再滑动的时候提示需要等待。
    if(s_slidingCount >= 2)
    {
        WinInforListDialog::instance()->showMsg(tr(SLIDING_PLATE_NOT_ALLOW));
        if(s_newestSlidingDateTime.secsTo(QDateTime::currentDateTime()) > 5)
        {
            s_slidingCount = 0;
        }
        return OutCount;
    }

    //发送滑动命令，发送前需要清空缓冲区
    tcflush(Communciation_Com::fd,TCIOFLUSH);

    long long slidingOrder = 0;
    if(direction == In)
    {
        slidingOrder = IN_SLIDING_PLATE;
    }else{
        slidingOrder = OUT_SLIDING_PLATE;
    }
    if(Communciation_Com::transmit(slidingOrder,4) < 0)
    {
        WinInforListDialog::instance()->showMsg(
                    tr(TRANSMIT_DATA_ERROR)
                    + Communciation_Com::fd
                    );
        if(s_newestSlidingDateTime.secsTo(QDateTime::currentDateTime()) > 5)
        {
            s_slidingCount = 0;
        }
        return TransmitErr;
    }

    //接收数据并判断数据是否正确
    QString receiveData = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);

    int m_sliderState = UnKnowErr;
    if(receiveData == NULL)
    {
        ErrorCountSave::instance()->addCount(6);
        WinInforListDialog::instance()->showMsg(
                    tr(SLIDING_PLATE_NO_CHANGE_TEXT)
            #ifdef DEBUG
                    + tr("未接收到数据")
            #endif
                    );
        m_sliderState = ReceiveNull;
    }
    else if(receiveData[1] == (char)0x31)
    {
        setButtonsState(Reference);

        m_sliderState = SlidingSuccess;
    }
    else if(receiveData[1] == (char)0x32)
    {
        setButtonsState(SampleToBeTested);

        m_sliderState = SlidingSuccess;
    }
    else if( receiveData[1] == (char)0x33)
    {
        WinInforListDialog::instance()->showMsg(
                    tr(MACHINE_MALFUNCTION_TEXT)
            #ifdef DEBUG
                    + tr("recv 0x33")
            #endif
                    );

        setButtonsState(Malfunction);

        m_sliderState = Malfunction;
    }else
    {
        ErrorCountSave::instance()->addCount(6);
        WinInforListDialog::instance()->showMsg(
                    tr(SLIDING_PLATE_NO_CHANGE_TEXT)
                    + ":"
                    + receiveData
                    );

        setButtonsState(Malfunction);

        m_sliderState = UnKnowErr;
    }
    return m_sliderState;
}

void Sliding::setSliderButtonPair(QPushButton *in, QPushButton *out)
{
    Q_ASSERT(in != NULL);
    Q_ASSERT(out != NULL);

    if(!s_moveInSliderButtons.contains(in))
    {
        s_moveInSliderButtons.append(in);
    }

    if(!s_moveOutSliderButtons.contains(out))
    {
        s_moveOutSliderButtons.append(out);
    }
}

void Sliding::setButtonsState(Sliding::SliderPosition position)
{
    bool inState = true;
    bool outState = true;

    //根据滑板位置设置移动滑板按钮状态。
    if(position == Reference)
    {
        inState = false;
        outState = true;
        issample::global_is_sample = Reference;
    }else if(position == SampleToBeTested)
    {
        inState = true;
        outState = false;
        issample::global_is_sample = SampleToBeTested;
    }else if(position == Malfunction)
    {
        inState = true;
        outState = true;
    }

    //使所有滑动滑板按钮的状态相同。
    foreach (QPushButton *button, s_moveOutSliderButtons)
    {
        button->setEnabled(outState);
    }
    foreach (QPushButton *button, s_moveInSliderButtons)
    {
        button->setEnabled(inState);
    }

    //记录这次移动滑板的时间和已连续滑动滑板的次数。连续滑动滑板是指此次移动滑板距离上次移动滑板未超过5秒
    if(s_newestSlidingDateTime.secsTo(QDateTime::currentDateTime()) < 5)
    {
        s_slidingCount++;
    }
    else
    {
        s_slidingCount = 0;
    }
    s_newestSlidingDateTime = QDateTime::currentDateTime();
}
