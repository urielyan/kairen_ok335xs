#ifndef DATASAVE_H
#define DATASAVE_H
#include <QApplication>
#include <QDebug>
#include <QSettings>

//relavnt calibrate measure
//记录标定了几组数据。0-12
#define MYSETTINGS_CALIBRATE_COUNT "calibratemeasurement_count"
#define MYSETTINGS_CALIBRATE_S_DATA(x) QString("s_count_data_").append(QString::number((x)))
#define MYSETTINGS_CALIBRATE_S_INPUT(x) QString("calibrate_input_s_").append(QString::number((x)))
//relavnt calibrate results
//标定数据下次保存的序号.1-20
#define MYSETTINGS_CALIBRATE_RESULT_COUNT "calibration_results_count"
#define MYSETTINGS_CALIBRATE_RESULT_RESULT(x) QString("calibration_results_in_result_").append(QString::number((x)))
#define MYSETTINGS_CALIBRATE_RESULT_DATA(x) QString("calibration_results_in_data_").append(QString::number((x)))

class AbstractDataSave
{
public:
  virtual QVariant value(QString key) = 0;
  virtual void setValue(QString key, qint64 value) = 0;
  virtual void setValue(QString key, QString value)  = 0;
  virtual void addCount(uint /*which*/) = 0;

};

class MeasurementDataSave : public AbstractDataSave
{

public:
  static MeasurementDataSave *instance();

public:
  bool test();
  QVariant value(QString key);
  void setValue(QString key, qint64 value);
  void setValue(QString key, QString value);
  void addCount(uint /*which*/);

protected:
  //virtual void init() = 0;
  QSettings *settings;

private:
  MeasurementDataSave();
  void init();
};

class ErrorCountSave : public AbstractDataSave
{
public:
  static ErrorCountSave *instance();

  void addCount(uint which);

public:
  bool test();
  QVariant value(QString key);
  void setValue(QString key, qint64 value);
  void setValue(QString key, QString value);

protected:
  //virtual void init() = 0;
  QSettings *settings;


private:
  ErrorCountSave();
  void init();
};
#endif // DATASAVE_H
