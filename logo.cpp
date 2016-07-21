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
    this->setFont(QFont(FONT_NAME, FONT_SIZE * 2,QFont::Normal));

//    ui->labelID->setText(tr("KL-3120"));
//    ui->labelName->setText(tr("Ｘ荧光油品硫分析仪"));
//    ui->labelCompute->setText(tr("上海凯仁电子仪器有限公司"));


    ui->labelID->setText(tr("SYD-17040"));
    ui->labelName->setText(tr("Ｘ荧光油品硫分析仪"));
    ui->labelCompute->setText(tr("上海昌吉地址仪器有限公司"));
}

logo::~logo()
{
    delete ui;
}
