#include "datasave.h"
#include "global.h"
DataSave *DataSave::instance()
{
  static DataSave _instance;
  return &_instance;
}

void DataSave::test()
{
  setValue("test", 1);
  qDebug() <<  settings->value("test") << settings->fileName();
  if(settings->value("test").toInt() != 1)
    {
      PRINT_DEBUG_INFOR;
      qDebug() << "QSettings err!";
  }
}

QVariant DataSave::value(QString key)
{
  return settings->value(key);
}

void DataSave::setValue(QString key, qint64 value)
{
  settings->setValue(key, value);
}

DataSave::DataSave()
{
  settings = new QSettings("./QSettings/test.settings", QSettings::NativeFormat);
  init();
  //  QCoreApplication::setOrganizationName("shanghaikairen");
  //  QCoreApplication::setApplicationName("analysis");
}

void DataSave::init()
{
  if(!settings->contains("calibratemeasurement_count_record")){
      settings->setValue("calibratemeasurement_count_record",1);
    }
  if((!settings->contains("calibration_results_in_result_1"))\
     && (!settings->contains("calibration_results_in_data_10"))){
      for(int tmp = 1;tmp <= 10;tmp++){
          //qDebug() << "calibrate_results";
          settings->setValue(QString("calibration_results_in_result_%1").arg(tmp)," ; ; ; ");
          settings->setValue(QString("calibration_results_in_data_%1").arg(tmp),"");
        }
    }

  QString tmpstr = "calibrate_input_s_";
  if(!settings->contains("calibrate_input_s_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          settings->setValue(tmpstr,"0.0000");
          tmpstr = "calibrate_input_s_";
        }
    }


  tmpstr = "s_count_data_";
  if(!settings->contains("s_count_data_1")){
      for(int tmpnumber = 0; tmpnumber < 12 ;tmpnumber++){
          tmpstr.append(QString("%1").arg(tmpnumber));
          qDebug() << tmpstr;
          settings->setValue(tmpstr,"");
          tmpstr = "s_count_data_";
      }
  }

  if(!settings->contains("work_curve_1") || !settings->contains("work_curve_9")){
      for(int i = 1;i <= 5 ; i++){
          settings->setValue(QString("work_curve_%1").arg(i),";;");
        }
      for(int i = 6 ; i <= 9 ; i++){
          settings->setValue(QString("work_curve_%1").arg(i),";;");
        }
    }


  //judge calibratemeasurement_count  whether is exit;if not exit create it.finally assign count variable.
  if(!settings->contains("calibratemeasurement_count")){
      settings->setValue("calibratemeasurement_count",0);
    }

  if(!settings->contains("count_voltage")){
      settings->setValue("count_voltage",578);
    }

  if(!settings->contains("light_voltage")){
      settings->setValue("light_voltage"," ");
    }

  if(!settings->contains("light_current")){
      settings->setValue("light_current"," ");
    }


  if(!settings->contains("machine_used_time")){
      settings->setValue("machine_used_time",0);
    }
  if(!settings->contains("most_use_time")){
      settings->setValue("most_use_time",0);
    }
  if (!settings->contains("passwd")){
      settings->setValue("passwd",111111);
  }

  // not set default value
//    if(!settings->contains("proportion_1"))
//      {
//        settings->setValue("proportion_1", 0.1);
//        settings->setValue("proportion_2", 1);
//      }


  if(!settings->contains("sample_count")){
      settings->setValue("sample_count",0);
  }
}
