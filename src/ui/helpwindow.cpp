#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QHelpEngine *pHelpEngine, const QUrl& pHomepage,
                       QDockWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::HelpWindow)
{
    mUi->setupUi(this);

    mHelpWidget = new HelpWidget(pHelpEngine);

    setWidget(mHelpWidget);
    setHomepage(pHomepage);
}

HelpWindow::~HelpWindow()
{
    delete mUi;
}

void HelpWindow::setHomepage(const QUrl& pHomepage)
{
    mHelpWidget->load(pHomepage);
}

void HelpWindow::setHelpWidgetTextSizeMultiplier(const double& pHelpWidgetTextSizeMultiplier)
{
    mHelpWidget->setTextSizeMultiplier(pHelpWidgetTextSizeMultiplier);
}

double HelpWindow::helpWidgetTextSizeMultiplier()
{
    return mHelpWidget->textSizeMultiplier();
}
