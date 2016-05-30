#ifndef SETUPDATETIME_H
#define SETUPDATETIME_H

#include <QWidget>
#include <QButtonGroup>
#include <QList>
#include <QList>

#include<QLabel>
#include <QDateTime>

class setupdatetime : public QWidget
{
    Q_OBJECT

public:
    enum ENUM_DATETIME
    {
        YEAR = 0,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
    };
    explicit setupdatetime(QWidget *parent = 0);
  void show_and_refresh();

private slots:
    void slotSaveButtonClicked();

    void slotButtonUpClicked(int id);
    void slotButtonDownClicked(int id);

private:
    QButtonGroup m_buttonGroupUp, m_buttonGroupDown;
    QList<QLabel*> m_labelList;

    void setLabelCurrentDateTime();
    void examineDayLabelIsRight();
    QString getSetDateTime();
};

class DateTimeFrame : public QFrame
{
    //useless
    Q_OBJECT
public:
    DateTimeFrame(QWidget *parent = 0);
};
#endif // SETUPDATETIME_H
