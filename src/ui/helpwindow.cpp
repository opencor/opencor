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

    mUi->contentsButton->setDefaultAction(mUi->actionContents);
    mUi->backButton->setDefaultAction(mUi->actionBack);
    mUi->forwardButton->setDefaultAction(mUi->actionForward);

    connect(mHelpWidget, SIGNAL(backAvailable(bool)),
            mUi->backButton, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(forwardAvailable(bool)),
            mUi->forwardButton, SLOT(setEnabled(bool)));
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

void HelpWindow::setZoomFactor(const double& pZoomFactor)
{
    mHelpWidget->setZoomFactor(pZoomFactor);
}

double HelpWindow::zoomFactor()
{
    return mHelpWidget->zoomFactor();
}

void HelpWindow::on_actionContents_triggered()
{
    mHelpWidget->gotoHomepage();
}

void HelpWindow::on_actionBack_triggered()
{
    mHelpWidget->back();
}

void HelpWindow::on_actionForward_triggered()
{
    mHelpWidget->forward();
}
