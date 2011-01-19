#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QHelpEngine *pHelpEngine, const QUrl& pHomePage,
                       QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::HelpWindow)
{
    mUi->setupUi(this);

    // Assign an action to the different 'toolbar' buttons

    mUi->homeButton->setDefaultAction(mUi->actionHome);

    mUi->backButton->setDefaultAction(mUi->actionBack);
    mUi->forwardButton->setDefaultAction(mUi->actionForward);

    mUi->normalSizeButton->setDefaultAction(mUi->actionNormalSize);

    mUi->zoomInButton->setDefaultAction(mUi->actionZoomIn);
    mUi->zoomOutButton->setDefaultAction(mUi->actionZoomOut);

    // Help widget

    mHelpWidget = new HelpWidget(pHelpEngine, pHomePage);

    mUi->verticalLayout->addWidget(mHelpWidget);

    // Some connections

    connect(mHelpWidget, SIGNAL(urlChanged(const QUrl&)),
            this, SLOT(checkUrlChanged(const QUrl&)));

    connect(mHelpWidget, SIGNAL(backAvailable(bool)),
            mUi->backButton, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(forwardAvailable(bool)),
            mUi->forwardButton, SLOT(setEnabled(bool)));

    connect(mHelpWidget, SIGNAL(zoomLevelChanged(int)),
            this, SLOT(checkNewZoomLevel(int)));
}

HelpWindow::~HelpWindow()
{
    delete mUi;
}

void HelpWindow::retranslateUi()
{
    mUi->retranslateUi(this);

    // Make sure that the enabled state of the various actions is correct
    // (indeed, to translate everything messes things up in that respect,
    // so...)

    mUi->actionHome->setEnabled(mHelpWidget->url() != mHelpWidget->homePage());

    mUi->actionBack->setEnabled(mHelpWidget->isBackAvailable());
    mUi->actionForward->setEnabled(mHelpWidget->isForwardAvailable());

    mUi->actionNormalSize->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->defaultZoomLevel());

    mUi->actionZoomOut->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->minimumZoomLevel());
}

void HelpWindow::defaultSettings()
{
    mHelpWidget->defaultSettings();
}

void HelpWindow::gotoHomePage()
{
    mHelpWidget->gotoHomePage();
}

int HelpWindow::defaultZoomLevel()
{
    return mHelpWidget->defaultZoomLevel();
}

void HelpWindow::setZoomLevel(const int& pZoomLevel)
{
    mHelpWidget->setZoomLevel(pZoomLevel);
}

int HelpWindow::zoomLevel()
{
    return mHelpWidget->zoomLevel();
}

void HelpWindow::on_actionHome_triggered()
{
    mHelpWidget->gotoHomePage();
}

void HelpWindow::on_actionBack_triggered()
{
    mHelpWidget->back();
}

void HelpWindow::on_actionForward_triggered()
{
    mHelpWidget->forward();
}

void HelpWindow::checkUrlChanged(const QUrl& pNewUrl)
{
    mUi->homeButton->setEnabled(pNewUrl != mHelpWidget->homePage());
}

void HelpWindow::checkNewZoomLevel(int pNewZoomLevel)
{
    mUi->actionNormalSize->setEnabled(pNewZoomLevel != mHelpWidget->defaultZoomLevel());

    mUi->actionZoomOut->setEnabled(pNewZoomLevel != mHelpWidget->minimumZoomLevel());
}

void HelpWindow::on_actionZoomIn_triggered()
{
    mHelpWidget->zoomIn();
}

void HelpWindow::on_actionZoomOut_triggered()
{
    mHelpWidget->zoomOut();
}

void HelpWindow::on_actionNormalSize_triggered()
{
    mHelpWidget->resetZoom();
}
