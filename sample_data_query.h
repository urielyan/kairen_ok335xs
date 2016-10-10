#ifndef SAMPLE_DATA_QUERY_H
#define SAMPLE_DATA_QUERY_H

#include <QDialog>
#include <QWidget>
#include <QSqlTableModel>
#include <QList>
namespace Ui {
class sample_data_query;
}

class QLabel;
class WinSpecifyIndexDialog : public QDialog
{
  Q_OBJECT
public:
  explicit WinSpecifyIndexDialog(const QModelIndex &index, QSqlTableModel *model,  QWidget *parent = 0);

private slots:
  void slotNextButtonClicked();
  void slotPreviousButtonClicked();

private:
  void init();
  void initData(QStringList valueList);

  QModelIndex m_index;
  QSqlTableModel *p_model;

  QList <QLabel *> m_labelList;
};
class WinSqlDataQuery : public QWidget
{
    Q_OBJECT

public:
    explicit WinSqlDataQuery(QWidget *parent = 0);
    ~WinSqlDataQuery();

    void show_and_refresh();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_b_datetime_clicked();

    void on_b_datetime_disorder_clicked();

    void on_b_sample_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::sample_data_query *ui;
    QSqlTableModel *p_model;

    void initTableview();
};

#endif // SAMPLE_DATA_QUERY_H
