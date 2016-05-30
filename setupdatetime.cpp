#include "setupdatetime.h"
#include "global.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

setupdatetime::setupdatetime(QWidget *parent) :
    QWidget(parent)
{
    //this->setFixedSize(800, 480);
    QHBoxLayout *p_HLayoutSetDateTime = new QHBoxLayout;

    QList<QLabel*> m_labelListUnit;
    for(int i = 0; i < 5; i++)
    {
        QVBoxLayout *p_VLayout = new QVBoxLayout;
        QPushButton *p_upButton =  new QPushButton(this);
        p_upButton->setObjectName("upbutton");
        m_buttonGroupUp.addButton(p_upButton, i);
        p_upButton->setText("+");


        QFrame *p_middleFrame = new QFrame(this);
        p_middleFrame->setObjectName("label");
        QHBoxLayout *p_HBoxLabel =  new QHBoxLayout(p_middleFrame);
        QLabel *p_Label = new QLabel(p_middleFrame);
        p_Label->setFont(QFont(FONT_NAME, FONT_SIZE));
        m_labelList.append(p_Label);
        p_Label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QLabel *p_LabelUnit = new QLabel(p_middleFrame);
        m_labelListUnit.append(p_LabelUnit);
        p_LabelUnit->setFont(QFont(FONT_NAME, FONT_SIZE - 10));
        p_LabelUnit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        p_HBoxLabel->addWidget(p_Label);
        p_HBoxLabel->addWidget(p_LabelUnit);

        QPushButton *p_bottomButton = new QPushButton(this);
        m_buttonGroupDown.addButton(p_bottomButton, i);
        p_bottomButton->setObjectName("bottombutton");
        p_bottomButton->setText("-");

        p_VLayout->addWidget(p_upButton);
        p_VLayout->addWidget(p_middleFrame);
        p_VLayout->addWidget(p_bottomButton);

        p_HLayoutSetDateTime->addLayout(p_VLayout);

    }
    m_labelListUnit.value(YEAR)->setText(tr("年"));
    m_labelListUnit.value(MONTH)->setText(tr("月"));
    m_labelListUnit.value(DAY)->setText(tr("日"));
    m_labelListUnit.value(HOUR)->setText(tr("时"));
    m_labelListUnit.value(MINUTE)->setText(tr("分"));
    connect(&m_buttonGroupUp, SIGNAL(buttonClicked(int)), this, SLOT(slotButtonUpClicked(int)));
    connect(&m_buttonGroupDown, SIGNAL(buttonClicked(int)), this, SLOT(slotButtonDownClicked(int)));

    QPushButton *p_cancelButton = new QPushButton(tr("取消"));
    connect(p_cancelButton, SIGNAL(clicked(bool)),this,SLOT(close()));

    QPushButton *p_SaveButton = new QPushButton(tr("保存"));
    connect(p_SaveButton, SIGNAL(clicked(bool)), this, SLOT(slotSaveButtonClicked()));

    QHBoxLayout *p_HLayoutCancelSave = new QHBoxLayout;
    p_HLayoutCancelSave->addWidget(p_cancelButton);
    p_HLayoutCancelSave->addWidget(p_SaveButton);

    QLabel *p_label = new QLabel(tr("设置时间"));
    p_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    p_label->setObjectName("title");
    p_label->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    QVBoxLayout *p_vLayoutMain = new QVBoxLayout(this);
    p_vLayoutMain->setSpacing(0);
    p_vLayoutMain->addWidget(p_label);
    p_vLayoutMain->addLayout(p_HLayoutSetDateTime);
    p_vLayoutMain->addLayout(p_HLayoutCancelSave);

    setLabelCurrentDateTime();
    this->setStyleSheet("QFrame#label{max-height: 80;min-height:50;"
                        "background-color:gray;border:1px solid, black; border-radius: 30px;}"
                        "QPushButton{min-height: 60;}"
                        "QPushButton#bottombutton, QPushButton#upbutton{text-align:center;}");
}

void setupdatetime::slotSaveButtonClicked()
{
    //ok button
    QMessageBox box;
    box.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    box.setText(tr("需要保存此次修改的日期和时间么？\n") + getSetDateTime());
    box.setStandardButtons(QMessageBox::Cancel|QMessageBox::Yes);
    int res = box.exec();
    if(res == QMessageBox::Yes){
        QString tmpstr("date -s ");
        tmpstr += " \"";
        tmpstr.append(getSetDateTime());
        tmpstr += "\"";
        int a = system(tmpstr.toLocal8Bit());
        int b = system("hwclock --systohc");
        QMessageBox msgbox;
        msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
        if(a== -1 || b == -1){
            msgbox.setText("设置时间不成功");
            msgbox.exec();
        }else{
            msgbox.setText("设置时间成功!");
            msgbox.exec();
        }
        this->close();
    }
}

void setupdatetime::show_and_refresh(){
    setLabelCurrentDateTime();
    examineDayLabelIsRight();
    this->showFullScreen();
}


void setupdatetime::slotButtonUpClicked(int id)
{
    int m_labelNumber = m_labelList.value(id)->text().toInt() + 1;
    switch (id) {
    case YEAR:
    {
        if(m_labelNumber > 2020)
            m_labelNumber = 2010;
        break;
    }
    case MONTH:
    {
        if(m_labelNumber > 12)
            m_labelNumber = 1;
        break;
    }
    case DAY:
    {
        int monthAry[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt() - 1;
        if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            monthAry[1] += 1;
        }

        if(m_labelNumber > monthAry[month])
            m_labelNumber = 1;
        break;
    }
    case HOUR:
    {
        if(m_labelNumber > 23)
            m_labelNumber = 0;
        break;
    }
    case MINUTE:
    {
        if(m_labelNumber > 59)
            m_labelNumber = 0;
        break;
    }

    default:
        break;
    }
    m_labelList.value(id)->setText(QString::number(m_labelNumber));
    examineDayLabelIsRight();
}

void setupdatetime::slotButtonDownClicked(int id)
{
    int m_labelNumber = m_labelList.value(id)->text().toInt() - 1;
    switch (id) {
    case YEAR:
    {
        if(m_labelNumber  < 2010)
            m_labelNumber = 2020;
        break;
    }
    case MONTH:
    {
        if(m_labelNumber < 1)
            m_labelNumber = 12;
        break;
    }
    case DAY:
    {
        int monthAry[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt() - 1;
        if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            monthAry[1] += 1;
        }

        if(m_labelNumber < 1)
            m_labelNumber = monthAry[month];
        break;
    }
    case HOUR:
    {
        if(m_labelNumber < 0)
            m_labelNumber = 23;
        break;
    }
    case MINUTE:
    {
        if(m_labelNumber < 0)
            m_labelNumber = 59;
        break;
    }

    default:
        break;
    }
    m_labelList.value(id)->setText(QString::number(m_labelNumber));
    examineDayLabelIsRight();
}

void setupdatetime::setLabelCurrentDateTime()
{
    m_labelList[YEAR]->setText(QString::number(QDate::currentDate().year()));
    m_labelList[MONTH]->setText(QString::number(QDate::currentDate().month()));
    m_labelList[DAY]->setText(QString::number(QDate::currentDate().day()));
    m_labelList[HOUR]->setText(QString::number(QTime::currentTime().hour()));
    m_labelList[MINUTE]->setText(QString::number(QTime::currentTime().minute()));
}

void setupdatetime::examineDayLabelIsRight()
{
    if(m_labelList[DAY]->text().toInt() >= 29)
    {
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt();

        //不是闰年：二月最大的数是28
        if((2 ==  month)&&
                !((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        {
            m_labelList[DAY]->setText("28");
        }
    }
}

QString setupdatetime::getSetDateTime()
{
    return QString("%1-%2-%3 %4:%5:00").arg(m_labelList[YEAR]->text()).arg(m_labelList[MONTH]->text())
            .arg(m_labelList[DAY]->text()).arg(m_labelList[HOUR]->text()).arg(m_labelList[MINUTE]->text());
}

DateTimeFrame::DateTimeFrame(QWidget *parent) :
    QFrame(parent)
{
    QLabel *p_Label = new QLabel(this);
    p_Label->setAlignment(Qt::AlignRight);

    QLabel *p_LabelUnit = new QLabel(this);
    p_LabelUnit->setAlignment(Qt::AlignLeft);

    QHBoxLayout *p_HBoxLabel =  new QHBoxLayout(this);
    p_HBoxLabel->addWidget(p_Label);
    p_HBoxLabel->addWidget(p_LabelUnit);

    this->setObjectName("DateTimeFrame");
    this->setStyleSheet(QString("QFrame#DateTimeFrame{"
                        "background-color:blue;"
                        "border:1px solid, black; "
                        "border-radius: %1px;}").arg(10));
}
