#ifndef ISSAMPLE_H
#define ISSAMPLE_H

#include <QWidget>

namespace Ui {
class issample;
}

class issample : public QWidget
{
    Q_OBJECT

public:
    explicit issample(QWidget *parent = 0);
    ~issample();

    static int global_is_sample;
    void change_label_content(int);

private slots:
    void change_label_content();
private:
    Ui::issample *ui;
};

#endif // ISSAMPLE_H
