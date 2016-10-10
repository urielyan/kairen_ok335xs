#ifndef INPUT_PERSON_SAMPLESERIAL_H
#define INPUT_PERSON_SAMPLESERIAL_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class input_person_sampleSerial;
}

class input_person_sampleSerial : public QDialog
{
    Q_OBJECT
public:
    static input_person_sampleSerial* instance();
    explicit input_person_sampleSerial(QWidget *parent = 0);
    ~input_person_sampleSerial();

    QString getSample();
    QString getPeople();

    void showPeopleSample();
    void showPeople();
    void showSample();

signals:
    void transmit_data(QString);

private slots:
    void on_b_return_clicked();

    void slotNumKeyPressed();

    void on_b_ok_clicked();

    void on_pushButton_clicked();

private:
    Ui::input_person_sampleSerial *ui;

    void initWidget();
};

#endif // INPUT_PERSON_SAMPLESERIAL_H
