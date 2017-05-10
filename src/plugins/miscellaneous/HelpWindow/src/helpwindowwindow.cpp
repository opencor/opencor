/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

#include <QClipboard>
#include <QHelpEngine>
#include <QMenu>
#include <QPoint>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QSettings>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

HelpWindowWindow::HelpWindowWindow(QWidget *pParent) :
    WindowWidget(pParent),
    mGui(new Ui::HelpWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

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

    mHelpWindowWidget = new HelpWindowWidget(this);

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

    mHelpWindowWidget->webView()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mHelpWindowWidget->webView(), SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    // Some connections to update the enabled state of our various actions

    connect(mHelpWindowWidget, SIGNAL(homePage(const bool &)),
            mGui->actionHome, SLOT(setDisabled(bool)));

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

    mHelpWindowWidget->webView()->back();
}

//==============================================================================

void HelpWindowWindow::on_actionForward_triggered()
{
    // Go to the next help page

    mHelpWindowWidget->webView()->forward();
}

//==============================================================================

void HelpWindowWindow::on_actionCopy_triggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mHelpWindowWidget->webView()->selectedText());
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
        mHelpWindowWidget->webView()->print(&printer);
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
