/*
 *1.此程序数据保存采用QSettings类，数据保存在～/.config/shanghaikairen/下，共有两个文件 ，一个是analysis.conf和count_data.conf
 *  其中analysis.conf中保存的内容有：
 *      calibrate_input_s_n：保存用户输入的标定样硫的数据。
 *      calibratemeasurement_count：记录正在标定第几组数据。会随着清除标定数据使他变为0，
 *              每次按标定的开始键就会自加，若此次标定不成功就会减一保持原值。
 *      calibratemeasurement_count_record:最近的标定结果记录，是一个轮巡，从1到10循环。保存的数是下一次计算kb值的所要记录在哪里的标记。
 *      calibration_results_in_data_n：一个标定结果所需要的所有数据，有(S%/reference/calibrate;S%/reference/calibrate...)
 *              共count number组数据，count number对应于下面的calibration_results_in_result_n那组数据中的count number
 *      calibration_results_in_result_n：标定的计算kb值的结果，包括（datetime;work curve;kbr(a0.a1,12);count number）
 *      passwd：密码
 *      s_count_data_n：记录标定时标定样和参考样的数据(标定样/参考样)
 *      sample_count:总共测量过多少次含量测量
 *      sample_data_n：含量测量记录的数据(work curve; datetime;measurement time;
 *                                    repeat time; average value;standard deviation)
 *      work_curve_n：工作曲线记录的数据（kb值或a0,a1,a2）;example:(k=0;b=0r=0    or    a0=1;a1=1;a2=1)
 *      count_voltage : 设定计数管高压
 *      light_voltage   : 设定光管高压
 *      light_current   : 设定光管电流
 *  count_data.conf中保存的内容有：
 *      count_count:
 *      count_data_n:
 *
 *  2.
*/
#include "widget.h"
#include "logo.h"
#include "printer.h"
#include "global.h"
#include "buzzer.h"

#include <QApplication>
#include <QTextCodec>
#include<QMessageBox>
#include <QTimer>
#include<QObject>
#include <QFontDatabase>
#include <QTranslator>
#include <QDebug>
#include <QSettings>


void settingsInit()
{

  QCoreApplication::setOrganizationName("shanghaikairen");
  QCoreApplication::setApplicationName("analysis");

  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, "/home/yange/test/");

  QSettings mysettings;
  if(!mysettings.contains("count_voltage")){
      mysettings.setValue("count_voltage",578);
    }

  if(!mysettings.contains("light_voltage")){
      mysettings.setValue("light_voltage",0);
    }

  if(!mysettings.contains("light_current")){
      mysettings.setValue("light_current",0);
    }
}

void LanguageInit()
{

#ifdef  Q_WS_QPA
  QMessageBox msgBox;
  msgBox.setText("The document has been modified.");
  msgBox.setInformativeText("Do you want to save your changes?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  int ret = msgBox.exec();
#endif

#if 1
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif


  int nIndex = QFontDatabase::addApplicationFont("/wenquanyi.ttf");//opt/Qtopia4.4.3/lib/fonts
  if (nIndex != -1){
      QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
      if (strList.count() > 0){
          QFont fontThis(strList.at(0));
          qDebug()<< strList.size() <<strList.at(0);
          fontThis.setPointSize(9);
          a.setFont(fontThis);
        }
    }
  nIndex = QFontDatabase::addApplicationFont("/symbol.ttf");//opt/Qtopia4.4.3/lib/fonts
  if (nIndex != -1){
      QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
      if (strList.count() > 0){
          QFont fontThis(strList.at(0));
          qDebug()<< strList.size() <<strList.at(0);
          fontThis.setPointSize(9);
          a.setFont(fontThis);
        }
    }

  //把QMessageBox的按钮变成中文的
  QTranslator trans;
  trans.load("/qt_zh_CN");
  a.installTranslator(&trans);

}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  settingsInit();
  LanguageInit();

  buzzer buz;
  buz.stop_music();

  printer pri;
#if 0
  printer::transmit(0x0A,1);
  printer::transmit((void *)"shanghaikairen",14);
  printer::transmit(0x0A,1);
#endif

  Widget w;
  logo l;
  l.showFullScreen();
  QTimer::singleShot(3000,&l,SLOT(close()));
  w.setFont(QFont("wenquanyi",FONT_SIZE,QFont::Normal));
  QTimer::singleShot(2800,&w,SLOT(showFullScreen()));
  return a.exec();
}
