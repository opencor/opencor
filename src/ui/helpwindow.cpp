#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QHelpEngine *pHelpEngine, const QUrl& pHomepage,
                       QDockWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::HelpWindow)
{
    mUi->setupUi(this);

    mHelpWidget = new HelpWidget(pHelpEngine, pHomepage);

    mUi->verticalLayout->addWidget(mHelpWidget);
}

HelpWindow::~HelpWindow()
{
    delete mUi;
}

void HelpWindow::resetAll()
{
    mHelpWidget->resetAll();
}

void HelpWindow::gotoHomepage()
{
    mHelpWidget->gotoHomepage();
}

void HelpWindow::setHelpWidgetTextSizeMultiplier(const double& pHelpWidgetTextSizeMultiplier)
{
    mHelpWidget->setTextSizeMultiplier(pHelpWidgetTextSizeMultiplier);
}

double HelpWindow::helpWidgetTextSizeMultiplier()
{
    return mHelpWidget->textSizeMultiplier();
}

void HelpWindow::on_homepageButton_clicked()
{
    mHelpWidget->gotoHomepage();
}

void HelpWindow::on_backwardButton_clicked()
{
    mHelpWidget->back();
}

void HelpWindow::on_forwardButton_clicked()
{
    mHelpWidget->forward();
}
