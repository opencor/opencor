#include "centralwidget.h"
#include "tabwidget.h"

#include "ui_centralwidget.h"

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add our tab widget

    mTabWidget = new TabWidget(this);

    mUi->verticalLayout->addWidget(mTabWidget);
}

CentralWidget::~CentralWidget()
{
    // Delete the UI

    delete mUi;
}
