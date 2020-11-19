#include "logo.h"
#include "ui_logo.h"
#include "global.h"

#include <QImage>
#include <QPixmap>

logo::logo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logo)
{
    ui->setupUi(this);
    this->setFont(QFont(FONT_NAME, FONT_SIZE * 3,QFont::Normal));

//    ui->labelID->setText(tr("KL-3120"));
//    ui->labelName->setText(tr("Ｘ荧光油品硫分析仪"));
//    ui->labelCompute->setText(tr("上海凯仁电子仪器有限公司"));


//    ui->labelID->setText(tr("SYD-17040"));
//    ui->labelName->setText(tr("Ｘ荧光油品硫分析仪"));
//    ui->labelCompute->setText(tr("上海昌吉地址仪器有限公司"));

    //2017.07.08 :为肖总的部队产品
//    ui->labelID->setText(tr("JY-9020-J"));
//    ui->labelName->setText(tr("硫含量测定仪"));
//    ui->labelCompute->setText(tr("上海景颐自动化设备有限公司"));

    //xuezong
//    ui->labelID->setText(tr("YZ--6180A"));
//    ui->labelName->setText(tr("X荧光测硫仪"));
//    ui->labelCompute->setText(tr("上海荧展电子仪器有限公司"));

//    ui->labelID->setText(tr("THA-6000XS"));
//    ui->labelName->setText(tr("X荧光定硫仪"));
//    ui->labelCompute->setText(tr("泰州金航分析仪器有限公司"));

    ui->labelID->setText("JY-9020");
    ui->labelName->setText("微量快速硫含量测试仪");
    ui->labelCompute->setText("上海阳光科学仪器制造有限公司");
}

logo::~logo()
{
    delete ui;
}
