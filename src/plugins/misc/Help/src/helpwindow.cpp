//==============================================================================
// Help window
//==============================================================================

#include "coreutils.h"
#include "docktoolbar.h"
#include "helpwindow.h"
#include "helpwidget.h"

//==============================================================================

#include "ui_helpwindow.h"

//==============================================================================

#include <QClipboard>
#include <QDir>
#include <QHelpEngine>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

static const QString OpencorHelpHomepageUrl = "qthelp://opencor/doc/user/index.html";

//==============================================================================

HelpWindow::HelpWindow(QWidget *pParent) :
    DockWidget(pParent),
    mUi(new Ui::HelpWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Extract the help files

    QString applicationBaseFileName =  QDir::tempPath()+QDir::separator()
                                      +QFileInfo(qApp->applicationFilePath()).baseName();

    mQchFileName = applicationBaseFileName+".qch";
    mQhcFileName = applicationBaseFileName+".qhc";

    Core::saveResourceAs(":qchFile", mQchFileName);
    Core::saveResourceAs(":qhcFile", mQhcFileName);

    // Set up the help engine

    mHelpEngine = new QHelpEngine(mQhcFileName);

    // Create a toolbar with different buttons
    // Note: this sadly can't be done using the design mode, so...

    Core::DockToolBar *toolbar = new Core::DockToolBar(this);

    toolbar->addAction(mUi->actionHome);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionBack);
    toolbar->addAction(mUi->actionForward);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionCopy);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionNormalSize);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionZoomIn);
    toolbar->addAction(mUi->actionZoomOut);
    toolbar->addSeparator();
    toolbar->addAction(mUi->actionPrint);

    mUi->verticalLayout->addWidget(toolbar);

    // Create and add the help widget

    mHelpWidget = new HelpWidget("HelpWidget", mHelpEngine,
                                 OpencorHelpHomepageUrl, this);

    mUi->verticalLayout->addWidget(mHelpWidget);

    // We want our own context menu for the help widget (indeed, we don't want
    // the default one which has the reload menu item and not the other actions
    // that we have in our toolbar, so...)

    mHelpWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mHelpWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(customContextMenu(const QPoint &)));

    // Some connections

    connect(mHelpWidget, SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(needUpdateActions()));

    connect(mHelpWidget, SIGNAL(backAvailable(bool)),
            mUi->actionBack, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(forwardAvailable(bool)),
            mUi->actionForward, SLOT(setEnabled(bool)));

    connect(mHelpWidget->page(), SIGNAL(selectionChanged()),
            this, SLOT(needUpdateActions()));

    connect(mHelpWidget, SIGNAL(zoomLevelChanged(int)),
            this, SLOT(needUpdateActions()));
}

//==============================================================================

HelpWindow::~HelpWindow()
{
    // Delete some internal objects

    delete mHelpEngine;
    delete mUi;

    // Delete the help files

    QFile(mQchFileName).remove();
    QFile(mQhcFileName).remove();
}

//==============================================================================

void HelpWindow::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    mUi->actionHome->setEnabled(mHelpWidget->url() != mHelpWidget->homePage());

    mUi->actionBack->setEnabled(mHelpWidget->isBackAvailable());
    mUi->actionForward->setEnabled(mHelpWidget->isForwardAvailable());

    mUi->actionCopy->setEnabled(!mHelpWidget->selectedText().isEmpty());

    mUi->actionNormalSize->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->defaultZoomLevel());

    mUi->actionZoomOut->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->minimumZoomLevel());
}

//==============================================================================

void HelpWindow::retranslateUi()
{
    // Retranslate the whole window

    mUi->retranslateUi(this);

    // Retranslate the help widget

    mHelpWidget->retranslateUi();
}

//==============================================================================

void HelpWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the help widget

    pSettings->beginGroup(mHelpWidget->objectName());
        mHelpWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the help widget

    pSettings->beginGroup(mHelpWidget->objectName());
        mHelpWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindow::gotoHomePage() const
{
    // Go to the home page

    mHelpWidget->gotoHomePage();
}

//==============================================================================

int HelpWindow::defaultZoomLevel() const
{
    // Return the default zoom level for the help widget

    return mHelpWidget->defaultZoomLevel();
}

//==============================================================================

void HelpWindow::setZoomLevel(const int &pZoomLevel) const
{
    // Set the zoom level for the help widget

    mHelpWidget->setZoomLevel(pZoomLevel);
}

//==============================================================================

int HelpWindow::zoomLevel() const
{
    // Return the current zoom level for the help widget

    return mHelpWidget->zoomLevel();
}

//==============================================================================

void HelpWindow::on_actionHome_triggered()
{
    // Go to the home page

    mHelpWidget->gotoHomePage();
}

//==============================================================================

void HelpWindow::on_actionBack_triggered()
{
    // Go to the previous help page

    mHelpWidget->back();
}

//==============================================================================

void HelpWindow::on_actionForward_triggered()
{
    // Go to the next help page

    mHelpWidget->forward();
}

//==============================================================================

void HelpWindow::on_actionCopy_triggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mHelpWidget->selectedText());
}

//==============================================================================

void HelpWindow::on_actionNormalSize_triggered()
{
    // Reset the zoom level of the help page contents

    mHelpWidget->resetZoom();
}

//==============================================================================

void HelpWindow::on_actionZoomIn_triggered()
{
    // Zoom in the help page contents

    mHelpWidget->zoomIn();
}

//==============================================================================

void HelpWindow::on_actionZoomOut_triggered()
{
    // Zoom out the help page contents

    mHelpWidget->zoomOut();
}

//==============================================================================

void HelpWindow::on_actionPrint_triggered()
{
    // Retrieve the printer with which the user wants to print the help page
    // and print it, should s/he still want to go ahead with the printing

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer);

    if ( dialog->exec() == QDialog::Accepted)
          mHelpWidget->print(&printer);
}

//==============================================================================

void HelpWindow::customContextMenu(const QPoint &) const
{
    // Create a custom context menu which items match the contents of our
    // toolbar

    QMenu menu;

    menu.addAction(mUi->actionHome);
    menu.addSeparator();
    menu.addAction(mUi->actionBack);
    menu.addAction(mUi->actionForward);
    menu.addSeparator();
    menu.addAction(mUi->actionCopy);
    menu.addSeparator();
    menu.addAction(mUi->actionNormalSize);
    menu.addSeparator();
    menu.addAction(mUi->actionZoomIn);
    menu.addAction(mUi->actionZoomOut);
    menu.addSeparator();
    menu.addAction(mUi->actionPrint);

    menu.exec(QCursor::pos());
}

//==============================================================================

void HelpWindow::needUpdateActions()
{
    // Something requires the actions to be udpated

    updateActions();
}

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
