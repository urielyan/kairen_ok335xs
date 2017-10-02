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
    ui->labelID->setText(tr("JY-9020-J"));
    ui->labelName->setText(tr("硫含量测定仪"));
    ui->labelCompute->setText(tr("上海景颐自动化设备有限公司"));
}

logo::~logo()
{
    delete ui;
}
