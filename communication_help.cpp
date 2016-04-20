#include "communication_help.h"
#include "ui_communication_help.h"
#include "global.h"

#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>

communication_help::communication_help(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::communication_help)
{
  ui->setupUi(this);
  QSettings communication_err_data("shanghaikairen","communication_error");
  if(!communication_err_data.contains("change_count_voltage_count")){
      communication_err_data.setValue("change_count_voltage_count",1);
    }
  if(!communication_err_data.contains("com_err_1")){
      for(int i = 1;i <= 11;i++){
          communication_err_data.setValue(QString("com_err_%1").arg(i),0);
        }
    }

  this->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  QList<QLabel *> labellist = this->findChildren<QLabel *>();
  for (int i = 0; i < labellist.count(); ++i) {
      labellist[i]->setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    }
  ui->label->setFont(QFont(FONT_NAME, FONT_SIZE*2,QFont::Normal));
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
      //QTextStream::
      int display_number = mysettings.value("calibratemeasurement_count_record").toInt() - 1;
      for(int i = 0;i < CALIBRATE_RESULT_MAX_RECORD;i++){
          //得到一个下一次标定会记录在哪的数，所以减一就是最新的数
          QStringList strlist = mysettings.value(QString("calibration_results_in_result_%1").arg(display_number)).toString().split(";");
          if(strlist.size() != 4)break;
          out << strlist[0] << ": "  << strlist[2] << "   ->   " << strlist[1] << "\n   "  << mysettings.value(QString("calibration_results_in_data_%1").arg(display_number)).toString() <<"\n \n";
          if(--display_number ==  0)display_number = CALIBRATE_RESULT_MAX_RECORD;
        }
      //out <<"标定结果1:" << mysettings.value("calibration_results_in_result_1").toString() << "\n标定1所需数据:" << mysettings.value("calibration_results_in_data_1").toString() <<"\n";
      //out << "The magic number is: " << 49 << "\n";
      file1.close();




      //      system("rm /home/yange/sample.txt");
      //      QFile file2("/home/yange/sample.txt");
      system("rm /media/sda1/sample.txt");
      QFile file2("/media/sda1/sample.txt");
      if (!file2.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
      QTextStream out2(&file2);
      QString query_str;
      query_str = QString("SELECT * FROM sample_data");
      QSqlQuery query;
      bool ok = query.exec(query_str);
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
          QMessageBox msgbox;
          msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          msgbox.setText("已经生成3个文件在u盘中：\n1,含量测量的数据库文件samplemeasurement.db\n2,含量测量的文本文件sample.txt\n3,标定结果文件calibrate.txt");
          msgbox.exec();
        }

    }else{
      QMessageBox msgbox;
      msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
      msgbox.setText("请确定已插入u盘");
      msgbox.exec();
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
      if(system("ls /media/sda1/new") == 0 &&  system("ls /dev/sda1") == 0){
          int res1 = system("cp  /media/sda1/new  /");

//          if(system("ls /media/mmcblk0p1/new") == 0 &&  system("ls /dev/mmcblk0") == 0){
//              int res1 = system("cp  /media/mmcblk0p1/new /");

          if(res1 == 0){
              system("reboot");
//              QMessageBox msgbox;
//              msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
//              msgbox.setText("需要重启使更新生效");
            }else{
              QMessageBox msgbox;
              msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
              msgbox.setText("不能复制");
              msgbox.exec();
            }
        }else{
          QMessageBox msgbox;
          msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
          msgbox.setText("请确定：\n1、已插入u盘；\n2、u盘中含有new这个更新文件");
          msgbox.exec();
        }
    }
}

void communication_help::on_pushButton_5_clicked()
{
  this->close();
}

void communication_help::on_pushButton_2_clicked()
{
  QSettings communication_err_data("shanghaikairen","communication_error");
  if(!communication_err_data.contains("com_err_1")){
      for(int i = 1;i <= 11;i++){
          communication_err_data.setValue(QString("com_err_%1").arg(i),0);
        }
    }
  int err1 = communication_err_data.value("com_err_1").toInt();
  int err2 = communication_err_data.value("com_err_2").toInt();
  int err3 = communication_err_data.value("com_err_3").toInt();
  int err4 = communication_err_data.value("com_err_4").toInt();
  //int err5 = communication_err_data.value("com_err_5").toInt();//setup
  int err6 = communication_err_data.value("com_err_6").toInt();
  int err10 = communication_err_data.value("com_err_10").toInt();
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText(QString("能谱:%1 计数:%2 含量:%3 标定:%4\n移动滑板:%5 停止测量:%6 设定:%7")
                 .arg(err1).arg(err2).arg(err3).arg(err4).arg(err6).arg(err10).arg(5));
  msgbox.exec();
}
