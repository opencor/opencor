#include "centralwidget.h"

#include "ui_centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::CentralWidget)
{
    mUi->setupUi(this);
}

CentralWidget::~CentralWidget()
{
    delete mUi;
}
