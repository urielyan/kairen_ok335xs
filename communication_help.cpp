#include "communication_help.h"
#include "ui_communication_help.h"
#include "global.h"
#include "datasave.h"
#include "database.h"

#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>

communication_help::communication_help(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::communication_help)
{
  ui->setupUi(this);

  INIT_LABEL_SIZE_FONT;
  ui->lableTitle->setFont(QFont(FONT_NAME, FONT_SIZE * 2 ,QFont::Normal));
  ui->lableTitle->setObjectName("title");
  ui->lableVersion->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

communication_help::~communication_help()
{
  delete ui;
}

void communication_help::on_pushButton_clicked()
{
  //copy file

  if(system("ls /dev/sda1")  == 0){
      //            system("rm  /home/yange/calibrate_sample.txt");
      //            QFile file1("/home/yange/calibrate_sample.txt");
      system("rm /media/sda1/calibrate.txt");
      QFile file1("/media/sda1/calibrate.txt");
      if (!file1.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
      QTextStream out(&file1);

      int display_number = MeasurementDataSave::instance()->value(MYSETTINGS_CALIBRATE_RESULT_COUNT).toInt() - 1;
      for(int i = 0;i < CALIBRATE_RESULT_MAX_RECORD;i++){
          //得到一个下一次标定会记录在哪的数，所以减一就是最新的数
          QString calibrateResults = MeasurementDataSave::instance()->value(MYSETTINGS_CALIBRATE_RESULT_RESULT(display_number)).toString();
          QStringList strlist = calibrateResults.split(";");
          if(strlist.size() != 4)
            break;
          QString calibrateDatakey = MYSETTINGS_CALIBRATE_RESULT_DATA(display_number);
          out << strlist[0] << ": "  << strlist[2] << "   ->   " << strlist[1] << "\n   "  << MeasurementDataSave::instance()->value(calibrateDatakey).toString() <<"\n \n";
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

      if(system("cp /samplemeasurement.db  /media/sda1/") ==0){
          WinInforListDialog::instance()->showMsg(tr("已经生成3个文件在u盘中：\n"
                                                     "1,含量测量的数据库文件samplemeasurement.db\n"
                                                     "2,含量测量的文本文件sample.txt\n"
                                                     "3,标定结果文件calibrate.txt"));
        }

    }else{
      WinInforListDialog::instance()->showMsg(tr("请确定已插入u盘"));
    }
}

void communication_help::on_pushButton_3_clicked()
{
  //product upgrade
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText("将会更新本系统，更新后会重新启动!确定更新请按确定,否则按取消!");
  msgbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
  int ret = msgbox.exec();
  if(ret == QMessageBox::Ok){
      QString udiskPath;
      if(GlobalData::instance()->strUdiskPath.size() > 0)
        udiskPath = GlobalData::instance()->strUdiskPath;
#ifdef FORLIN_OK335XS
      if(system("ls /media/sda1/new") == 0 &&  system("ls /dev/sda1") == 0){
          int res1 = system("cp  /media/sda1/new  /");
#endif
#ifdef FRIENDLYARM_TINY210
          if(system("ls /udisk/new") == 0){
              int res1 = system("cp  /udisk/new /");
#endif
          if(res1 == 0){
              system("reboot");
            }else{
              WinInforListDialog::instance()->showMsg(tr("不能复制"));
            }
        }else{
              WinInforListDialog::instance()->showMsg(tr("请确定: 1、已插入u盘; 2、u盘中含有new这个更新文件."));
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

  WinInforListDialog::instance()->showMsg(tr("能谱:%1 计数:%2 含量:%3 标定:%4\n移动滑板:%5 停止测量:%6 设定:%7")
                                                           .arg(err1).arg(err2).arg(err3).arg(err4).arg(err6).arg(err10).arg(5));
}
