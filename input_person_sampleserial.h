#ifndef INPUT_PERSON_SAMPLESERIAL_H
#define INPUT_PERSON_SAMPLESERIAL_H

#include <QWidget>

namespace Ui {
class input_person_sampleSerial;
}

class input_person_sampleSerial : public QWidget
{
    Q_OBJECT

signals:
    void transmit_data(QString);

public:
    static input_person_sampleSerial* instance();
    explicit input_person_sampleSerial(QWidget *parent = 0);
    ~input_person_sampleSerial();

    static QString line_people,line_serial;

    void just_show_people();
    void just_show_sample();
    void initData();
private slots:
    void on_b_return_clicked();

    void slot_keyNumPressed();

    void on_b_ok_clicked();

    void on_pushButton_clicked();

private:
    Ui::input_person_sampleSerial *ui;

};

#endif // INPUT_PERSON_SAMPLESERIAL_H
