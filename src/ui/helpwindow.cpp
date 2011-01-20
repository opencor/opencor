#include <QMessageBox>

#include "helpwindow.h"
#include "helpwidget.h"

#include "ui_helpwindow.h"

#include <QClipboard>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>

HelpWindow::HelpWindow(QHelpEngine *pHelpEngine, const QUrl& pHomePage,
                       QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::HelpWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Assign an action to the different 'toolbar' buttons

    mUi->homeButton->setDefaultAction(mUi->actionHome);

    mUi->backButton->setDefaultAction(mUi->actionBack);
    mUi->forwardButton->setDefaultAction(mUi->actionForward);

    mUi->copyButton->setDefaultAction(mUi->actionCopy);

    mUi->normalSizeButton->setDefaultAction(mUi->actionNormalSize);

    mUi->zoomInButton->setDefaultAction(mUi->actionZoomIn);
    mUi->zoomOutButton->setDefaultAction(mUi->actionZoomOut);

    mUi->printButton->setDefaultAction(mUi->actionPrint);

    // Create and add the help widget

    mHelpWidget = new HelpWidget(pHelpEngine, pHomePage);

    mUi->verticalLayout->addWidget(mHelpWidget);

    // Prevent objects from being dropped on the help widget

    mHelpWidget->setAcceptDrops(false);

    // We want our own context menu for the help widget (indeed, we don't want
    // the default one which has the reload menu item and not the other actions
    // that we have in our 'toolbar', so...)

    mHelpWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Some connections

    connect(mHelpWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(customContextMenu(const QPoint&)));

    connect(mHelpWidget, SIGNAL(urlChanged(const QUrl&)),
            this, SLOT(newUrl(const QUrl&)));

    connect(mHelpWidget, SIGNAL(backAvailable(bool)),
            mUi->actionBack, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(forwardAvailable(bool)),
            mUi->actionForward, SLOT(setEnabled(bool)));

    connect(mHelpWidget->page(), SIGNAL(selectionChanged()),
            this, SLOT(newSelection()));

    connect(mHelpWidget, SIGNAL(zoomLevelChanged(int)),
            this, SLOT(newZoomLevel(int)));
}

HelpWindow::~HelpWindow()
{
    // Delete the UI

    delete mUi;
}

void HelpWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Make sure that the enabled state of the various actions is correct
    // (indeed, to translate everything messes things up in that respect,
    // so...)

    mUi->actionHome->setEnabled(mHelpWidget->url() != mHelpWidget->homePage());

    mUi->actionBack->setEnabled(mHelpWidget->isBackAvailable());
    mUi->actionForward->setEnabled(mHelpWidget->isForwardAvailable());

    mUi->actionCopy->setEnabled(!mHelpWidget->selectedText().isEmpty());

    mUi->actionNormalSize->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->defaultZoomLevel());

    mUi->actionZoomOut->setEnabled(mHelpWidget->zoomLevel() != mHelpWidget->minimumZoomLevel());
}

void HelpWindow::defaultSettings()
{
    // Set the default settings for the help widget

    mHelpWidget->defaultSettings();
}

void HelpWindow::gotoHomePage()
{
    // Go to the home page

    mHelpWidget->gotoHomePage();
}

int HelpWindow::defaultZoomLevel()
{
    // Return the default zoom level for the help widget

    return mHelpWidget->defaultZoomLevel();
}

void HelpWindow::setZoomLevel(const int& pZoomLevel)
{
    // Set the zoom level for the help widget

    mHelpWidget->setZoomLevel(pZoomLevel);
}

int HelpWindow::zoomLevel()
{
    // Return the current zoom level for the help widget

    return mHelpWidget->zoomLevel();
}

void HelpWindow::on_actionHome_triggered()
{
    // Go to the home page

    mHelpWidget->gotoHomePage();
}

void HelpWindow::on_actionBack_triggered()
{
    // Go to the previous help page

    mHelpWidget->back();
}

void HelpWindow::on_actionForward_triggered()
{
    // Go to the next help page

    mHelpWidget->forward();
}

void HelpWindow::on_actionCopy_triggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mHelpWidget->selectedText());
}

void HelpWindow::on_actionNormalSize_triggered()
{
    // Reset the zoom level of the help page contents

    mHelpWidget->resetZoom();
}

void HelpWindow::on_actionZoomIn_triggered()
{
    // Zoom in the help page contents

    mHelpWidget->zoomIn();
}

void HelpWindow::on_actionZoomOut_triggered()
{
    // Zoom out the help page contents

    mHelpWidget->zoomOut();
}

void HelpWindow::on_actionPrint_triggered()
{
    // Retrieve the printer with which the user wants to print the help page
    // and print it, should s/he still want to go ahead with the printing

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer);

    if ( dialog->exec() == QDialog::Accepted)
          mHelpWidget->print(&printer);
}

void HelpWindow::customContextMenu(const QPoint& pPos)
{
    // Create a custom context menu which items match the contents of our
    // 'toolbar'

    QMenu *menu = new QMenu;

    menu->addAction(mUi->actionHome);
    menu->addSeparator();
    menu->addAction(mUi->actionBack);
    menu->addAction(mUi->actionForward);
    menu->addSeparator();
    menu->addAction(mUi->actionCopy);
    menu->addSeparator();
    menu->addAction(mUi->actionNormalSize);
    menu->addSeparator();
    menu->addAction(mUi->actionZoomIn);
    menu->addAction(mUi->actionZoomOut);
    menu->addSeparator();
    menu->addAction(mUi->actionPrint);

    menu->exec(mapToGlobal(pPos));
}

void HelpWindow::newUrl(const QUrl& pNewUrl)
{
    // The help page contents has changed, so determine whether the home action
    // should now be enabled or not

    mUi->actionHome->setEnabled(pNewUrl != mHelpWidget->homePage());
}

void HelpWindow::newSelection()
{
    // The selection in the help widget has changed, so determine whether the
    // copy action should now be enabled or not

    mUi->actionCopy->setEnabled(!mHelpWidget->selectedText().isEmpty());
}

void HelpWindow::newZoomLevel(int pNewZoomLevel)
{
    // The help page contents has been zoomed in/out or reset, so update the
    // relevant actions

    mUi->actionNormalSize->setEnabled(pNewZoomLevel != mHelpWidget->defaultZoomLevel());

    mUi->actionZoomOut->setEnabled(pNewZoomLevel != mHelpWidget->minimumZoomLevel());
}
