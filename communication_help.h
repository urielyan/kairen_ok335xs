#ifndef COMMUNICATION_HELP_H
#define COMMUNICATION_HELP_H

#include <QWidget>

namespace Ui {
class communication_help;
}

class AbstractDataSave;
class communication_help : public QWidget
{
    Q_OBJECT

public:
    explicit communication_help(QWidget *parent = 0);
    ~communication_help();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

private:
    bool saveCalibrateData(QString path);
    bool saveSampleData(QString path);
    bool sendSampleDataToPC();
    bool copySampleDatabase(QString dir);

private:
    Ui::communication_help *ui;

    AbstractDataSave *p_mysettings;

    QString m_uDiskDir;
};

#endif // COMMUNICATION_HELP_H
