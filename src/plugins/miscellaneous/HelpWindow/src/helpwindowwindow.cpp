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
#include "helpwindowwidget.h"
#include "helpwindowwindow.h"
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

    connect(mGui->actionHome, &QAction::triggered,
            this, &HelpWindowWindow::actionHomeTriggered);
    connect(mGui->actionBack, &QAction::triggered,
            this, &HelpWindowWindow::actionBackTriggered);
    connect(mGui->actionForward, &QAction::triggered,
            this, &HelpWindowWindow::actionForwardTriggered);
    connect(mGui->actionCopy, &QAction::triggered,
            this, &HelpWindowWindow::actionCopyTriggered);
    connect(mGui->actionNormalSize, &QAction::triggered,
            this, &HelpWindowWindow::actionNormalSizeTriggered);
    connect(mGui->actionZoomIn, &QAction::triggered,
            this, &HelpWindowWindow::actionZoomInTriggered);
    connect(mGui->actionZoomOut, &QAction::triggered,
            this, &HelpWindowWindow::actionZoomOutTriggered);
    connect(mGui->actionPrint, &QAction::triggered,
            this, &HelpWindowWindow::actionPrintTriggered);

    // Create a tool bar widget with different buttons

    auto toolBarWidget = new Core::ToolBarWidget();

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
#else
    mGui->layout->addWidget(new Core::BorderedWidget(mHelpWindowWidget,
                                                     true, false, false, false));
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

    connect(mHelpWindowWidget->webView(), &QWebView::customContextMenuRequested,
            this, &HelpWindowWindow::showCustomContextMenu);

    // Some connections to update the enabled state of our various actions

    connect(mHelpWindowWidget, QOverload<bool>::of(&HelpWindowWidget::homePage),
            mGui->actionHome, &QAction::setDisabled);

    connect(mHelpWindowWidget, &HelpWindowWidget::backEnabled,
            mGui->actionBack, &QAction::setEnabled);
    connect(mHelpWindowWidget, &HelpWindowWidget::forwardEnabled,
            mGui->actionForward, &QAction::setEnabled);

    connect(mHelpWindowWidget, &HelpWindowWidget::defaultZoomLevel,
            mGui->actionNormalSize, &QAction::setDisabled);
    connect(mHelpWindowWidget, &HelpWindowWidget::zoomingOutEnabled,
            mGui->actionZoomOut, &QAction::setEnabled);

    connect(mHelpWindowWidget, &HelpWindowWidget::copyTextEnabled,
            mGui->actionCopy, &QAction::setEnabled);

    // En/disable the printing action, depending on whether printers are
    // available

    mGui->actionPrint->setEnabled(!QPrinterInfo::availablePrinterNames().isEmpty());
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

void HelpWindowWindow::loadSettings(QSettings &pSettings)
{
    // Retrieve the settings of our help window widget

    pSettings.beginGroup(mHelpWindowWidget->objectName());
        mHelpWindowWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void HelpWindowWindow::saveSettings(QSettings &pSettings) const
{
    // Keep track of the settings of our help window widget

    pSettings.beginGroup(mHelpWindowWidget->objectName());
        mHelpWindowWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void HelpWindowWindow::actionHomeTriggered()
{
    // Go to the home page

    mHelpWindowWidget->goToHomePage();
}

//==============================================================================

void HelpWindowWindow::actionBackTriggered()
{
    // Go to the previous help page

    mHelpWindowWidget->webView()->back();
}

//==============================================================================

void HelpWindowWindow::actionForwardTriggered()
{
    // Go to the next help page

    mHelpWindowWidget->webView()->forward();
}

//==============================================================================

void HelpWindowWindow::actionCopyTriggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mHelpWindowWidget->webView()->selectedText());
}

//==============================================================================

void HelpWindowWindow::actionNormalSizeTriggered()
{
    // Reset the zoom level of the help page contents

    mHelpWindowWidget->resetZoom();
}

//==============================================================================

void HelpWindowWindow::actionZoomInTriggered()
{
    // Zoom in the help page contents

    mHelpWindowWidget->zoomIn();
}

//==============================================================================

void HelpWindowWindow::actionZoomOutTriggered()
{
    // Zoom out the help page contents

    mHelpWindowWidget->zoomOut();
}

//==============================================================================

void HelpWindowWindow::actionPrintTriggered()
{
    // Retrieve the printer with which the user wants to print the help page
    // and print it, should s/he still want to go ahead with the printing

    QPrinter printer;
    QPrintDialog printDialog(&printer);

    if (printDialog.exec() == QDialog::Accepted) {
        mHelpWindowWidget->webView()->print(&printer);
    }
}

//==============================================================================

void HelpWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

} // namespace HelpWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
