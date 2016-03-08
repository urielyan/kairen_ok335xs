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
}

logo::~logo()
{
    delete ui;
}
