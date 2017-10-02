#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include <QDir>
#include <QProcess>

#include "communication_help.h"
#include "ui_communication_help.h"
#include "global.h"
#include "datasave.h"
#include "database.h"
#include "wininforlistdialog.h"

communication_help::communication_help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::communication_help)
  , p_mysettings(NULL)
  #ifdef FORLIN_OK335XS
  , m_uDiskDir("/media/sda1/")
  #endif
  #ifdef FRIENDLYARM_TINY210
  , m_uDiskDir("/home/yange/")
  #endif
{
    ui->setupUi(this);

    INIT_LABEL_SIZE_FONT;
    ui->lableTitle->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
    ui->lableTitle->setObjectName("title");
    ui->lableVersion->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    p_mysettings = ErrorCountSave::instance();

    //阳光那边给部队的可以发送数据到上位机的版本(最初未经测试版) :"当前版本：Ｖ2.0(2017.07.09)"

    //v2.1 :修改预热时间为一个小时，添加读取下位机版本的功能；去掉稳峰记录的计数数据保存．
    QString versionText;
    versionText = QString("当前版本：Ｖ2.1(2017.10.02)\n%1")
            .arg(getSubMachineVersion());
    ui->lableVersion->setText(versionText);
}

communication_help::~communication_help()
{
    delete ui;
}

void communication_help::on_pushButton_clicked()
{
    //copy file

    //sendSampleDataToPC();

    if(!QDir(m_uDiskDir).exists())
    {
        WinInforListDialog::instance()->showMsg(tr("请确定已插入u盘"));
        return;
    }

    bool saveCalibrateDataResults= saveCalibrateData(m_uDiskDir + "calibrate.txt");
    bool saveSampleDataResults = saveSampleData(m_uDiskDir + "sample.txt");
    bool saveDatabaseResults = copySampleDatabase(m_uDiskDir);

    if(saveCalibrateDataResults && saveDatabaseResults && saveSampleDataResults)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("已经生成3个文件在u盘中：\n"
                       "1,含量测量的数据库文件samplemeasurement.db\n"
                       "2,含量测量的文本文件sample.txt\n"
                       "3,标定结果文件calibrate.txt"
                       )
                    );
    }

#if 0
    //            system("rm  /home/yange/calibrate_sample.txt");
    //            QFile file1("/home/yange/calibrate_sample.txt");
    system("rm /media/sda1/calibrate.txt");
    QFile file1("/media/sda1/calibrate.txt");
    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
    QTextStream out(&file1);

    int display_number = p_mysettings->value(MYSETTINGS_CALIBRATE_RESULT_COUNT).toInt() - 1;
    for(int i = 0;i < CALIBRATE_RESULT_MAX_RECORD;i++)
    {
        //得到一个下一次标定会记录在哪的数，所以减一就是最新的数
        QString calibrateResults = p_mysettings->value(MYSETTINGS_CALIBRATE_RESULT_RESULT(display_number)).toString();
        QStringList strlist = calibrateResults.split(";");
        if(strlist.size() != 4)
            break;
        QString calibrateDatakey = MYSETTINGS_CALIBRATE_RESULT_DATA(display_number);
        out << strlist[0] << ": "  << strlist[2] << "   ->   " << strlist[1] << "\n   "  << p_mysettings->value(calibrateDatakey).toString() <<"\n \n";
        if(--display_number ==  0)
        {
            display_number = CALIBRATE_RESULT_MAX_RECORD;
        }
    }
    file1.close();



    //      system("rm /home/yange/sample.txt");
    //      QFile file2("/home/yange/sample.txt");
    system("rm /media/sda1/sample.txt");
    QFile file2("/media/sda1/sample.txt");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
    QTextStream out2(&file2);
    QSqlQuery query(Database::instance()->getDb());
    bool ok = query.exec("SELECT * FROM sample_data");
    if(ok == true ){
        while(query.next()) {
            QString msgstr;
            for(int i = 0;i < 10;i++){
                //msgstr += strlist[i];
                //                  msgstr += QString::number(i + 1);
                //                  msgstr += " : ";
                if(query.value(i).toString() == NULL){
                    msgstr += "NULL";
                    msgstr += "/";
                    continue;
                }else if(query.value(i).toString().compare(" 是")){
                    msgstr += "Y";
                    msgstr += "/";
                }else if(query.value(i).toString().compare("否")){
                    msgstr += "N";
                    msgstr += "/";
                }
                msgstr += query.value(i).toString();
                msgstr += "/";
            }
            msgstr.chop(1);
            out2 << msgstr << "\n\n";
        }
    }
    file2.close();

    if(system("cp ./samplemeasurement.db  /media/sda1/") ==0)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("已经生成3个文件在u盘中：\n"
                       "1,含量测量的数据库文件samplemeasurement.db\n"
                       "2,含量测量的文本文件sample.txt\n"
                       "3,标定结果文件calibrate.txt"
                       )
                    );
    }
#endif
}

void communication_help::on_pushButton_3_clicked()
{
    //product upgrade
    QMessageBox msgbox;
    msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    msgbox.setText(tr("将会更新本系统，更新后会重新启动!确定更新请按确定,否则按取消!"));
    msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    int ret = msgbox.exec();
    if(ret == QMessageBox::Ok)
    {
        QFile upgrageFile(m_uDiskDir + "new");
        if(upgrageFile.exists())
        {
            QStringList args;
            args << m_uDiskDir + "/new" << "/";

            QProcess process;
            process.start("cp", args);

            if(!process.waitForFinished())
            {
                WinInforListDialog::instance()->showMsg(
                            tr("拷贝文件失败。")
                            );

                return;
            }

            system("reboot");;
        }
        else
        {
            WinInforListDialog::instance()->showMsg(
                        tr("请确定: 1、已插入u盘; 2、u盘中含有new这个更新文件.")
                        );
        }
    }
}

void communication_help::on_pushButton_5_clicked()
{
    this->close();
}

void communication_help::on_pushButton_2_clicked()
{
    ErrorCountSave::instance()->addCount(1);
    int err1 = ErrorCountSave::instance()->value("com_err_1").toInt();
    int err2 = ErrorCountSave::instance()->value("com_err_2").toInt();
    int err3 = ErrorCountSave::instance()->value("com_err_3").toInt();
    int err4 = ErrorCountSave::instance()->value("com_err_4").toInt();
    //int err5 = ErrorCountSave::instance()->value("com_err_5").toInt();//setup
    int err6 = ErrorCountSave::instance()->value("com_err_6").toInt();
    int err10 = ErrorCountSave::instance()->value("com_err_10").toInt();

    WinInforListDialog::instance()->showMsg(
                tr("能谱:%1 计数:%2 含量:%3 标定:%4\n移动滑板:%5 停止测量:%6 设定:%7")
                .arg(err1).arg(err2).arg(err3).arg(err4).arg(err6).arg(err10).arg(5)
                );
}

bool communication_help::saveCalibrateData(QString path)
{
    QFile file1(path);
    if (!file1.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        WinInforListDialog::instance()->showMsg(
                    tr("生成标定文件失败")
                    );
        return false;
    }
    file1.resize(0);

    QTextStream out(&file1);

    int display_number = p_mysettings->value(MYSETTINGS_CALIBRATE_RESULT_COUNT).toInt() - 1;
    for(int i = 0; i < CALIBRATE_RESULT_MAX_RECORD;i++)
    {
        //得到一个下一次标定会记录在哪的数，所以减一就是最新的数
        QString calibrateResults = p_mysettings->value(MYSETTINGS_CALIBRATE_RESULT_RESULT(display_number)).toString();
        QStringList strlist = calibrateResults.split(";");
        if(strlist.size() != 4)
            break;
        QString calibrateDatakey = MYSETTINGS_CALIBRATE_RESULT_DATA(display_number);
        out << strlist[0] << ": "  << strlist[2] << "   ->   " << strlist[1] << "\n   "  << p_mysettings->value(calibrateDatakey).toString() <<"\n \n";
        if(--display_number ==  0)
        {
            display_number = CALIBRATE_RESULT_MAX_RECORD;
        }
    }
    file1.close();

    return true;
}

bool communication_help::saveSampleData(QString path)
{
    QFile file2(path);
    if (!file2.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        WinInforListDialog::instance()->showMsg(
                    tr("生成数据库文件失败")
                    );
        return false;
    }
    file2.resize(0);

    QTextStream out2(&file2);
    QSqlQuery query(Database::instance()->getDb());
    bool ok = query.exec("SELECT * FROM sample_data");
    if(ok == true )
    {
        while(query.next())
        {
            QString msgstr;
            for(int i = 0;i < 10;i++)
            {
                if(query.value(i).toString() == NULL)
                {
                    msgstr += "NULL";
                    msgstr += "/";
                    continue;
                }
#if 0
                else if(query.value(i).toString().compare("是"))
                {
                    msgstr += "Y";
                    msgstr += "/";
                }
                else if(query.value(i).toString().compare("否"))
                {
                    msgstr += "N";
                    msgstr += "/";
                }
#endif
                msgstr += query.value(i).toString();
                msgstr += "/";
            }
            msgstr.chop(1);
            out2 << msgstr << "\n\n";
        }
    }
    file2.close();

    return true;
}

#include "com.h"

bool communication_help::sendSampleDataToPC()
{
    QString out2;
    QSqlQuery query(Database::instance()->getDb());
    bool ok = query.exec("SELECT * FROM sample_data");
    if(ok == true )
    {
        while(query.next())
        {
            QString msgstr;
            for(int i = 0;i < 10;i++)
            {
                if(query.value(i).toString() == NULL)
                {
                    msgstr += "NULL";
                    msgstr += "|";
                    continue;
                }
                msgstr += query.value(i).toString();
                msgstr += "|";
            }
            msgstr.chop(1);
            out2.append(msgstr).append("\n\n");
        }
    }

    SendSampleDataToPC::sendData(out2);
    return true;
}

bool communication_help::copySampleDatabase(QString dir)
{
    QStringList args;
    args << "./samplemeasurement.db" << dir;

    QProcess process;
    process.start("cp", args);

    if(! process.waitForFinished())
    {
        WinInforListDialog::instance()->showMsg(
                    tr("拷贝数据库文件失败。")
                    );
        return false;
    }

    return true;
}

QString communication_help::getSubMachineVersion()
{
    if (Communciation_Com::transmit(GET_VERSION, 3) < 0)
    {
        return QString("0.0");
      }
    QString recv_data = Communciation_Com::receive(SLIDING_PLATE_CHANGE_TIME);
    recv_data.remove(0, 1);
    recv_data.chop(1);
    if(recv_data == NULL){
        return QString("0.00");
    }

    return recv_data;
}
