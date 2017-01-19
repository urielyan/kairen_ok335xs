/*arm-linux-g++ -lts -Wl,-O1 -Wl,-rpath,/usr/local/arm/qt4.8.5/lib -o new main.o widget.o date_widget.o samplemeasurement.o countingmeasurement.o issample.o ispreheat.o spectrummeasurement.o showcountingmeasurement.o dataquery.o countdataquery.o systemsetup.o passwd.o setupdatetime.o showsamplemeasurement.o calibration.o calibrationmeasurement.o caibrateresultquery.o input_s_percentage.o query_s_count_data.o showcalibratemeasurement.o count_kb_value.o modify_kb_value.o com.o samplemeasurementquery.o hide_system.o input_person_sampleserial.o sample_data_query.o logo.o proportion.o inspectoscope.o printer.o input_machine_use_time.o painter_histoym.o spectrum_painter.o buzzer.o communication_help.o query_change_voltage.o settings_datasave.o global.o wininforlistdialog.o moc_widget.o moc_date_widget.o moc_samplemeasurement.o moc_countingmeasurement.o moc_issample.o moc_ispreheat.o moc_spectrummeasurement.o moc_showcountingmeasurement.o moc_dataquery.o moc_countdataquery.o moc_systemsetup.o moc_passwd.o moc_setupdatetime.o moc_showsamplemeasurement.o moc_calibration.o moc_calibrationmeasurement.o moc_caibrateresultquery.o moc_input_s_percentage.o moc_query_s_count_data.o moc_showcalibratemeasurement.o moc_count_kb_value.o moc_modify_kb_value.o moc_com.o moc_samplemeasurementquery.o moc_hide_system.o moc_input_person_sampleserial.o moc_sample_data_query.o moc_logo.o moc_proportion.o moc_inspectoscope.o moc_printer.o moc_input_machine_use_time.o moc_painter_histogram.o moc_spectrum_painter.o moc_buzzer.o moc_communication_help.o moc_query_change_voltage.o moc_settings_datasave.o moc_wininforlistdialog.o    -L/usr/local/arm/qt4.8.5/lib -lQtSql -L/usr/local/arm/tslib/lib -L/usr/local/arm/qt4.8.5/lib -lQtGui -lQtNetwork -lQtCore -lpthread
yange@yange-Vostro-260:~/ok335XS/cross/kairen_ok335xs$ cp new /media/yange/E832-121D/
 *1.此程序数据保存采用QSettings类，数据保存在～/.config/shanghaikairen/下，共有两个文件 ，一个是analysis.conf和count_data.conf
 *  其中analysis.conf中保存的内容有：
 *    标定相关:
 *      calibratemeasurement_count：记录正在标定了几组数据。因为标定数据是从０开始的所以这个数也是标定下次记录的ID
 *              每次按标定的开始键就会自加，若此次标定不成功就会减一保持原值。
 *      calibration_results_count:最近的标定结果记录，是一个轮巡，从1到20循环。保存当计算kb值时的保存数据和结果的编号.例如为5时，下一个记录就是:
 *          calibration_results_in_data_5 and calibration_results_in_result_5
 *      calibration_results_in_data_n：一个标定结果所需要的所有数据，有(S%/reference/calibrate;S%/reference/calibrate...)
 *              共count number组数据，count number对应于下面的calibration_results_in_result_n那组数据中的count number
 *              for example (6 group):
 *                ("0.0021/1788/13012", "0.0049/1812/13011", "0.0120/1879/13008", "0.0207/1947/13006",
 *                "0.0550/2225/13005", "0.1006/2600/12990", "0.2522/3839/13035", "0.5043/5822/13041",
 *                "1.0027/9612/13046", "2.5016/19881/13036", "4.9992/33385/13006")
 *      calibration_results_in_result_n：标定的计算kb值的结果，包括（datetime;work curve;kbr(a0.a1,12);count number）
 *      s_count_data_n：记录标定时标定样和参考样的数据(标定样/参考样) 0-11
 *      calibrate_input_s_n：保存用户输入的标定样硫的数据。(0-12)
 *      work_curve_n：工作曲线记录的数据（kb值或a0,a1,a2）;example:(k=0;b=0r=0    or    a0=1;a1=1;a2=1)
 *
 *
 *    含量测量:
 *      sample_count:总共测量过多少次含量测量
 *      sample_data_n：含量测量记录的数据(work curve; datetime;measurement time;
 *                                    repeat time; average value;standard deviation)
 *
 *      count_voltage : 设定计数管高压
 *      light_voltage   : 设定光管高压
 *      light_current   : 设定光管电流
 *
 *      passwd：密码
 *  count_data.conf中保存的内容有：
 *      count_count:
 *      count_data_n:
 *
 *  2.数据库：
 *       create table sample_data(people_id TEXT,sample_serial TEXT,date_time DATE,wo    rk_curve INTEGER,measurement_time INTEGER,repeat_time INTEGER,average DOUBLE    ,deviation DOUBLE);
 *      insert into sample_data values(0,"000001","0000000000000","2015-3-13 10:50:30",1,15,3,0.2222,0.0001);
*/

#include "widget.h"
#include "logo.h"
#include "printer.h"
#include "global.h"
#include "buzzer.h"
#include "datasave.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QTextCodec>
#include<QMessageBox>
#include <QTimer>
#include<QObject>
#include <QFontDatabase>
#include <QTranslator>
#include <QDebug>
#include <QSettings>

#include <QSqlDatabase>
#include<QSqlDriver>

#ifdef __arm__
#ifdef FORLIN_OK335XS
#include <QWSServer>
#endif
#endif

void initSettings()
{

  QCoreApplication::setOrganizationName("shanghaikairen");
  QCoreApplication::setApplicationName("analysis");

  QSettings mysettings("shanghaikairen","analysis");
//  mysettings.setPath(QSettings::NativeFormat, QSettings::UserScope, "/shanghaikairen");
  qDebug() << mysettings.fileName();
  mysettings.clear();

  if(!mysettings.contains("calibration_results_count")){
      mysettings.setValue("calibration_results_count",1);
    }
  if((!mysettings.contains("calibration_results_in_result_1"))\
     && (!mysettings.contains("calibration_results_in_data_10"))){
      for(int tmp = 1;tmp <= 10;tmp++){
          //qDebug() << "calibrate_results";
          mysettings.setValue(QString("calibration_results_in_result_%1").arg(tmp)," ; ; ; ");
          mysettings.setValue(QString("calibration_results_in_data_%1").arg(tmp),"");
        }
    }

  QString tmpstr = "calibrate_input_s_";
  if(!mysettings.contains("calibrate_input_s_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          mysettings.setValue(tmpstr,"0.0000");
          tmpstr = "calibrate_input_s_";
        }
    }


  tmpstr = "s_count_data_";
  if(!mysettings.contains("s_count_data_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          qDebug() << tmpstr;
          mysettings.setValue(tmpstr,"");
          tmpstr = "s_count_data_";
      }
  }

  if(!mysettings.contains("work_curve_1") || !mysettings.contains("work_curve_9")){
      for(int i = 1;i <= 5 ; i++){
          mysettings.setValue(QString("work_curve_%1").arg(i),";;");
        }
      for(int i = 6 ; i <= 9 ; i++){
          mysettings.setValue(QString("work_curve_%1").arg(i),";;");
        }
    }


  //judge calibratemeasurement_count  whether is exit;if not exit create it.finally assign count variable.
  if(!mysettings.contains("calibratemeasurement_count")){
      mysettings.setValue("calibratemeasurement_count",0);
    }

  if(!mysettings.contains("count_voltage")){
      mysettings.setValue("count_voltage",578);
    }

  if(!mysettings.contains("light_voltage")){
      mysettings.setValue("light_voltage"," ");
    }

  if(!mysettings.contains("light_current")){
      mysettings.setValue("light_current"," ");
    }


  if(!mysettings.contains("machine_used_time")){
      mysettings.setValue("machine_used_time",0);
    }
  if(!mysettings.contains("most_use_time")){
      mysettings.setValue("most_use_time",0);
    }
  if (!mysettings.contains("passwd")){
      mysettings.setValue("passwd",111111);
  }

  // not set default value
//    if(!mysettings.contains("proportion_1"))
//      {
//        mysettings.setValue("proportion_1", 0.1);
//        mysettings.setValue("proportion_2", 1);
//      }


  if(!mysettings.contains("sample_count")){
      mysettings.setValue("sample_count",0);
  }
}

void initLanguage(QApplication &a)
{
#if 1
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  qDebug() << "test";
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, "/test/");
#endif

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

void initDatabase()
{
#if 0
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("/samplemeasurement.db");
  bool ok = db.open();
  if(ok == false){
      WinInforListDialog::instance()->showMsg(tr("不能打开含量测量的数据"));
    }
  //printf(",,,%s\n",__FUNCTION__);
  QSqlQuery query(db);
  ok = query.exec("SELECT * FROM sample_data;");
  if(ok == false){
      ok = query.exec("create table sample_data(people_id,sample_serial,date_time,work_curve,measurement_time,repeat_time,average,deviation,is_auto,current_coefficient);");
      if(ok == false){
          WinInforListDialog::instance()->showMsg(tr("不能打开含量测量的数据"));
          return;
        }
      WinInforListDialog::instance()->showMsg(tr("创建了含量测量的数据库"));
    }
#endif
}

int main(int argc, char *argv[])
{
  if(argc > 2)
    {
      GlobalData::instance()->strComName = QString(argv[1]);
      GlobalData::instance()->strUdiskPath = QString(argv[2]);
    }
  QApplication a(argc, argv);

  QTranslator tsor;           //创建翻译器
  tsor.load("/home/yange/en.qm");    //加载语言包
  QApplication::installTranslator(&tsor); //安装翻译器

#ifdef FRIENDLYARM_TINY210
  a.setStyleSheet(""
                  "QPushButton#stop{"
                  "background-color:(255, 0, 0); color: rgb(255, 255, 255); font-weight:bold;"
                  "}"
                  "QPushButton#start{background-color:rgb(0, 255, 0); color: rgb(255, 255, 255); font-weight:bold;}"  );

  //                          "min-width:80; font: 28px; min-height:60;"
  //                          "}"
  //                          "QPushButton#buttonTitle{"
  //                          "font: 48px;"
  //                          "min-height:50px;"
  //                          "}"

  //                          "QLabel{"
  //                          "font: 10px;"
  //                          "min-height:15px;"
  //                          "}"
  //                          "QLabel#countDown{"
  //                          "font: 15px;"
  //                          "min-height:20px;"
  //                          "}"
  //                          "QLabel#title{"
  //                          "font: 15px;"
  //                          "min-height:20px;"
  //                          "}"

  //                          "QComboBox {"
  //                          "min-height: 40px;"
  //                          "font: 30px;"
  //                          "}"

  //                          "QLineEdit{"
  //                          "font: 24px;"
  //                          "}"

  //                          "QTableView{"
  //                          "font:15px;"
  //                          "}"
  //                          "QTableView::item{"
  //                          "min-height: 16px;"
  //                          "}"
  //                          "QHeaderView{font:7px}"

  //                          "QScrollBar{width:10px;}"

  //                          "QMessageBox{min-height: 50px; min-width: 80px;}"

#endif

#ifdef FORLIN_OK335XS
  a.setStyleSheet(QString("QPushButton{"
                          "min-width:80; font: 28px; min-height:60;"
                          "}"
                          "QPushButton#stop{"
                          "background-color:rgb(0, 255, 0); color: rgb(255, 255, 255); font-weight:bold;"
                          "}"
                          "QPushButton#start{"
                          "background-color:rgb(0, 255, 0); color: rgb(255, 255, 255); font-weight:bold;"
                          "}"
                          "QPushButton#buttonTitle{"
                          "font: 48px;"
                          "min-height:50px;"
                          "}"

                          "QLabel{"
                          "font: 24px;"
                          "min-height:%1;"
                          "}"
                          "QLabel#countDown{"
                          "font: 48px;"
                          "min-height:50px;"
                          "}"
                          "QLabel#title{"
                          "font: 48px;"
                          "min-height:50px;"
                          "}"

                          "QComboBox {"
                          "min-height: 50px;"
                          "font: 25px;"
                          "}"

                          "QLineEdit{"
                          "font: 24px;"
                          "}"

                          "QTableView{"
                          "font:20px;"
                          "}"
                          "QTableView::item{"
                          "min-height: 18px;"
                          "}"
                          "QHeaderView{font:20px}"

                          "QScrollBar{width:10px;}"
                          ).arg(FONT_SIZE * 1.5));
#endif

  //initSettings();
  initLanguage(a);
  initDatabase();

#ifdef __arm__
#ifdef FORLIN_OK335XS
  QWSServer::setCursorVisible(false);//这句就能让我们实现触摸屏能用而光标又不显示的功能了。
  qDebug() << "set no cursor";
#endif
#endif
  buzzer buz;
  buz.stop_music();

  printer pri;
#if 0
  printer::transmit(0x0A,1);
  printer::transmit((void *)"shanghaikairen",14);
  printer::transmit(0x0A,1);
#endif

  Widget w;
  //  logo l;
  //  l.showFullScreen();
  //  QTimer::singleShot(5000,&l,SLOT(close()));
  //  w.setFont(QFont("wenquanyi",FONT_SIZE,QFont::Normal));
  //  QTimer::singleShot(4800,&w,SLOT(showFullScreen()));
  w.showFullScreen();
  return a.exec();
}
