//==============================================================================
// Help window
//==============================================================================

#include "coreutils.h"
#include "helpwindow.h"
#include "helpwidget.h"
#include "toolbarwidget.h"

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
    mGui(new Ui::HelpWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Extract the help files

    QString applicationBaseFileName =  QDir::tempPath()+QDir::separator()
                                      +QFileInfo(qApp->applicationFilePath()).baseName();

    mQchFileName = applicationBaseFileName+".qch";
    mQhcFileName = applicationBaseFileName+".qhc";

    Core::saveResourceAs(":Help_qchFile", mQchFileName);
    Core::saveResourceAs(":Help_qhcFile", mQhcFileName);

    // Set up the help engine

    mHelpEngine = new QHelpEngine(mQhcFileName);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionHome);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionBack);
    toolBarWidget->addAction(mGui->actionForward);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionCopy);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionNormalSize);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionZoomIn);
    toolBarWidget->addAction(mGui->actionZoomOut);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionPrint);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add the help widget

    mHelpWidget = new HelpWidget(mHelpEngine, OpencorHelpHomepageUrl, this);

    mHelpWidget->setObjectName("HelpWidget");

    mGui->layout->addWidget(mHelpWidget);

    // We want our own context menu for the help widget (indeed, we don't want
    // the default one which has the reload menu item and not the other actions
    // that we have in our tool bar widget, so...)

    mHelpWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mHelpWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Some connections to update the enabled state of our various actions

    connect(mHelpWidget, SIGNAL(notHomePage(const bool &)),
            mGui->actionHome, SLOT(setEnabled(bool)));

    connect(mHelpWidget, SIGNAL(backEnabled(const bool &)),
            mGui->actionBack, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(forwardEnabled(const bool &)),
            mGui->actionForward, SLOT(setEnabled(bool)));

    connect(mHelpWidget, SIGNAL(notDefaultZoomLevel(const bool &)),
            mGui->actionNormalSize, SLOT(setEnabled(bool)));
    connect(mHelpWidget, SIGNAL(zoomOutEnabled(const bool &)),
            mGui->actionZoomOut, SLOT(setEnabled(bool)));

    connect(mHelpWidget, SIGNAL(copyTextEnabled(const bool &)),
            mGui->actionCopy, SLOT(setEnabled(bool)));
}

//==============================================================================

HelpWindow::~HelpWindow()
{
    // Delete some internal objects

    delete mHelpEngine;

    // Delete the help files

    QFile(mQchFileName).remove();
    QFile(mQhcFileName).remove();

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void HelpWindow::retranslateUi()
{
    // Retranslate the whole window

    mGui->retranslateUi(this);

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

void HelpWindow::on_actionHome_triggered()
{
    // Go to the home page

    mHelpWidget->goToHomePage();
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

void HelpWindow::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Create a custom context menu which items match the contents of our
    // tool bar widget

    QMenu menu;

    menu.addAction(mGui->actionHome);
    menu.addSeparator();
    menu.addAction(mGui->actionBack);
    menu.addAction(mGui->actionForward);
    menu.addSeparator();
    menu.addAction(mGui->actionCopy);
    menu.addSeparator();
    menu.addAction(mGui->actionNormalSize);
    menu.addSeparator();
    menu.addAction(mGui->actionZoomIn);
    menu.addAction(mGui->actionZoomOut);
    menu.addSeparator();
    menu.addAction(mGui->actionPrint);

    menu.exec(QCursor::pos());
}

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
