/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Help window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "helpwindowwindow.h"
#include "helpwindowwidget.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_helpwindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QClipboard>
#include <QDir>
#include <QHelpEngine>
#include <QMenu>
#include <QPoint>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

static const auto OpencorHelpWindowHomepageUrl = QStringLiteral("qthelp://opencor/doc/user/index.html");

//==============================================================================

HelpWindowWindow::HelpWindowWindow(QWidget *pParent) :
    WindowWidget(pParent),
    mGui(new Ui::HelpWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Extract the help files

    QString applicationBaseFileName =  QDir::tempPath()+QDir::separator()
                                      +QFileInfo(qApp->applicationFilePath()).baseName();

    mQchFileName = applicationBaseFileName+".qch";
    mQhcFileName = applicationBaseFileName+".qhc";

    Core::writeResourceToFile(mQchFileName, ":HelpWindow_qchFile");
    Core::writeResourceToFile(mQhcFileName, ":HelpWindow_qhcFile");

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

    // Create and add a help window widget

    mHelpWindowWidget = new HelpWindowWidget(mHelpEngine, OpencorHelpWindowHomepageUrl, this);

    mHelpWindowWidget->setObjectName("HelpWindowWidget");

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mHelpWindowWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mHelpWindowWidget,
                                                     true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionHome);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionBack);
    mContextMenu->addAction(mGui->actionForward);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionCopy);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionNormalSize);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionZoomIn);
    mContextMenu->addAction(mGui->actionZoomOut);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionPrint);

    // We want our own context menu for our help window widget (indeed, we don't
    // want the default one, which has the reload menu item and not the other
    // actions that we have in our tool bar widget)

    mHelpWindowWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mHelpWindowWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    // Some connections to update the enabled state of our various actions

    connect(mHelpWindowWidget, SIGNAL(notHomePage(const bool &)),
            mGui->actionHome, SLOT(setEnabled(bool)));

    connect(mHelpWindowWidget, SIGNAL(backEnabled(const bool &)),
            mGui->actionBack, SLOT(setEnabled(bool)));
    connect(mHelpWindowWidget, SIGNAL(forwardEnabled(const bool &)),
            mGui->actionForward, SLOT(setEnabled(bool)));

    connect(mHelpWindowWidget, SIGNAL(defaultZoomLevel(const bool &)),
            mGui->actionNormalSize, SLOT(setDisabled(bool)));
    connect(mHelpWindowWidget, SIGNAL(zoomingOutEnabled(const bool &)),
            mGui->actionZoomOut, SLOT(setEnabled(bool)));

    connect(mHelpWindowWidget, SIGNAL(copyTextEnabled(const bool &)),
            mGui->actionCopy, SLOT(setEnabled(bool)));

    // En/disable the printing action, depending on whether printers are
    // available

    mGui->actionPrint->setEnabled(QPrinterInfo::availablePrinterNames().count());
}

//==============================================================================

HelpWindowWindow::~HelpWindowWindow()
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

void HelpWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);

    // Retranslate our help window widget

    mHelpWindowWidget->retranslateUi();
}

//==============================================================================

void HelpWindowWindow::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our help window widget

    pSettings->beginGroup(mHelpWindowWidget->objectName());
        mHelpWindowWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our help window widget

    pSettings->beginGroup(mHelpWindowWidget->objectName());
        mHelpWindowWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowWindow::on_actionHome_triggered()
{
    // Go to the home page

    mHelpWindowWidget->goToHomePage();
}

//==============================================================================

void HelpWindowWindow::on_actionBack_triggered()
{
    // Go to the previous help page

    mHelpWindowWidget->back();
}

//==============================================================================

void HelpWindowWindow::on_actionForward_triggered()
{
    // Go to the next help page

    mHelpWindowWidget->forward();
}

//==============================================================================

void HelpWindowWindow::on_actionCopy_triggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mHelpWindowWidget->selectedText());
}

//==============================================================================

void HelpWindowWindow::on_actionNormalSize_triggered()
{
    // Reset the zoom level of the help page contents

    mHelpWindowWidget->resetZoom();
}

//==============================================================================

void HelpWindowWindow::on_actionZoomIn_triggered()
{
    // Zoom in the help page contents

    mHelpWindowWidget->zoomIn();
}

//==============================================================================

void HelpWindowWindow::on_actionZoomOut_triggered()
{
    // Zoom out the help page contents

    mHelpWindowWidget->zoomOut();
}

//==============================================================================

void HelpWindowWindow::on_actionPrint_triggered()
{
    // Retrieve the printer with which the user wants to print the help page
    // and print it, should s/he still want to go ahead with the printing

    QPrinter printer;
    QPrintDialog printDialog(&printer);

    if (printDialog.exec() == QDialog::Accepted)
        mHelpWindowWidget->print(&printer);
}

//==============================================================================

void HelpWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
