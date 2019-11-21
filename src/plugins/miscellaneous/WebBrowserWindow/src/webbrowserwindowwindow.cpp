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
// Web Browser window
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"
#include "progressbarwidget.h"
#include "toolbarwidget.h"
#include "webbrowserwindowwidget.h"
#include "webbrowserwindowwindow.h"

//==============================================================================

#include "ui_webbrowserwindowwindow.h"

//==============================================================================

#include <QClipboard>
#include <QLineEdit>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QSettings>
#include <QTimer>
#include <QWebHistory>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

WebBrowserWindowWindow::WebBrowserWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::WebBrowserWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->actionClear, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionClearTriggered);
    connect(mGui->actionBack, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionBackTriggered);
    connect(mGui->actionForward, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionForwardTriggered);
    connect(mGui->actionCopy, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionCopyTriggered);
    connect(mGui->actionNormalSize, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionNormalSizeTriggered);
    connect(mGui->actionZoomIn, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionZoomInTriggered);
    connect(mGui->actionZoomOut, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionZoomOutTriggered);
    connect(mGui->actionPrint, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionPrintTriggered);
    connect(mGui->actionInspect, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionInspectTriggered);
    connect(mGui->actionReload, &QAction::triggered,
            this, &WebBrowserWindowWindow::actionReloadTriggered);

    // Create a tool bar widget with a URL value and refresh button
    // Note: the spacer is a little trick to improve the rendering of our tool
    //       bar widget...

    auto topToolBarWidget = new Core::ToolBarWidget();
    auto spacer = new QWidget(topToolBarWidget);

    spacer->setMinimumSize(0, 0);
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    mUrlValue = new QLineEdit(topToolBarWidget);

#ifdef Q_OS_MAC
    mUrlValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    connect(mUrlValue, &QLineEdit::returnPressed,
            this, &WebBrowserWindowWindow::returnPressed);

    topToolBarWidget->addWidget(spacer);
    topToolBarWidget->addWidget(mUrlValue);
    topToolBarWidget->addAction(mGui->actionReload);

    mGui->layout->addWidget(topToolBarWidget);

    // Make the URL value our focus proxy

    setFocusProxy(mUrlValue);

    // Create a tool bar widget with different buttons

    auto bottomToolBarWidget = new Core::ToolBarWidget();

    bottomToolBarWidget->addAction(mGui->actionClear);
    bottomToolBarWidget->addSeparator();
    bottomToolBarWidget->addAction(mGui->actionBack);
    bottomToolBarWidget->addAction(mGui->actionForward);
    bottomToolBarWidget->addSeparator();
    bottomToolBarWidget->addAction(mGui->actionCopy);
    bottomToolBarWidget->addSeparator();
    bottomToolBarWidget->addAction(mGui->actionNormalSize);
    bottomToolBarWidget->addSeparator();
    bottomToolBarWidget->addAction(mGui->actionZoomIn);
    bottomToolBarWidget->addAction(mGui->actionZoomOut);
    bottomToolBarWidget->addSeparator();
    bottomToolBarWidget->addAction(mGui->actionPrint);

    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(bottomToolBarWidget);

    // Create and add a Web browser window widget

    mWebBrowserWindowWidget = new WebBrowserWindowWidget(this);

    mWebBrowserWindowWidget->setObjectName("WebBrowserWindowWidget");

    mWebBrowserWindowWidget->showProgressBar();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mWebBrowserWindowWidget,
                                                     true, true, true, true));
#else
    mGui->layout->addWidget(new Core::BorderedWidget(mWebBrowserWindowWidget,
                                                     true, false, false, false));
#endif

    // Various connections to handle our Web browser window widget

    connect(mWebBrowserWindowWidget->webView(), &QWebView::urlChanged,
            this, &WebBrowserWindowWindow::urlChanged);

    // Create and populate our context menu

    mContextMenu = new QMenu(this);

    mContextMenu->addAction(mGui->actionClear);
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
    mContextMenu->addSeparator();
    mContextMenu->addAction(mGui->actionInspect);

    // We want our own context menu for our Web browser window widget (indeed,
    // we don't want the default one, which has the reload menu item and not the
    // other actions that we have in our tool bar widget)

    mWebBrowserWindowWidget->webView()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mWebBrowserWindowWidget->webView(), &QWebView::customContextMenuRequested,
            this, &WebBrowserWindowWindow::showCustomContextMenu);

    // Some connections to update the enabled state of our various actions

    connect(mWebBrowserWindowWidget, QOverload<bool>::of(&WebBrowserWindowWidget::homePage),
            mGui->actionClear, &QAction::setDisabled);

    connect(mWebBrowserWindowWidget, &WebBrowserWindowWidget::backEnabled,
            mGui->actionBack, &QAction::setEnabled);
    connect(mWebBrowserWindowWidget, &WebBrowserWindowWidget::forwardEnabled,
            mGui->actionForward, &QAction::setEnabled);

    connect(mWebBrowserWindowWidget, &WebBrowserWindowWidget::defaultZoomLevel,
            mGui->actionNormalSize, &QAction::setDisabled);
    connect(mWebBrowserWindowWidget, &WebBrowserWindowWidget::zoomingOutEnabled,
            mGui->actionZoomOut, &QAction::setEnabled);

    connect(mWebBrowserWindowWidget, &WebBrowserWindowWidget::copyTextEnabled,
            mGui->actionCopy, &QAction::setEnabled);

    // En/disable the printing action, depending on whether printers are
    // available

    mGui->actionPrint->setEnabled(!QPrinterInfo::availablePrinterNames().isEmpty());
}

//==============================================================================

WebBrowserWindowWindow::~WebBrowserWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void WebBrowserWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);
}

//==============================================================================

static const char *SettingsUrl = "Url";

//==============================================================================

void WebBrowserWindowWindow::loadSettings(QSettings &pSettings)
{
    // Retrieve our current URL (and load it)

    mUrlValue->setText(pSettings.value(SettingsUrl).toString());

    returnPressed();

    // Retrieve the settings of our Web browser window widget

    pSettings.beginGroup(mWebBrowserWindowWidget->objectName());
        mWebBrowserWindowWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void WebBrowserWindowWindow::saveSettings(QSettings &pSettings) const
{
    // Keep track of our current URL

    pSettings.setValue(SettingsUrl, mUrlValue->text());

    // Keep track of the settings of our Web browser window widget

    pSettings.beginGroup(mWebBrowserWindowWidget->objectName());
        mWebBrowserWindowWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void WebBrowserWindowWindow::urlChanged(const QUrl &pUrl)
{
    // The URL has changed, so update our URL value

    QString url = pUrl.toString();

    mUrlValue->setText((url != mWebBrowserWindowWidget->homePage())?url:QString());
}

//==============================================================================

void WebBrowserWindowWindow::actionClearTriggered()
{
    // Clear the contents of our Web browser window widget
    // Note: we disable the progress bar since we don't want to see its
    //       progress...

    mWebBrowserWindowWidget->progressBarWidget()->setEnabled(false);

    mWebBrowserWindowWidget->clear();
}

//==============================================================================

void WebBrowserWindowWindow::actionBackTriggered()
{
    // Go to the previous page
    // Note: we enable/disable the progress bar based on whether the back URL is
    //       our homepage since we don't want to see its progress in the latter
    //       case...

    mWebBrowserWindowWidget->progressBarWidget()->setEnabled(mWebBrowserWindowWidget->webView()->history()->backItem().url().toString() != mWebBrowserWindowWidget->homePage());

    mWebBrowserWindowWidget->webView()->back();
}

//==============================================================================

void WebBrowserWindowWindow::actionForwardTriggered()
{
    // Go to the next page
    // Note: we enable/disable the progress bar based on whether the forward URL
    //       is our homepage since we don't want to see its progress in the
    //       latter case...

    mWebBrowserWindowWidget->progressBarWidget()->setEnabled(mWebBrowserWindowWidget->webView()->history()->forwardItem().url().toString() != mWebBrowserWindowWidget->homePage());

    mWebBrowserWindowWidget->webView()->forward();
}

//==============================================================================

void WebBrowserWindowWindow::actionCopyTriggered()
{
    // Copy the current slection to the clipboard

    QApplication::clipboard()->setText(mWebBrowserWindowWidget->webView()->selectedText());
}

//==============================================================================

void WebBrowserWindowWindow::actionNormalSizeTriggered()
{
    // Reset the zoom level of the page contents

    mWebBrowserWindowWidget->resetZoom();
}

//==============================================================================

void WebBrowserWindowWindow::actionZoomInTriggered()
{
    // Zoom in the page contents

    mWebBrowserWindowWidget->zoomIn();
}

//==============================================================================

void WebBrowserWindowWindow::actionZoomOutTriggered()
{
    // Zoom out the page contents

    mWebBrowserWindowWidget->zoomOut();
}

//==============================================================================

void WebBrowserWindowWindow::actionPrintTriggered()
{
    // Retrieve the printer with which the user wants to print the page and
    // print it, should s/he still want to go ahead with the printing

    QPrinter printer;
    QPrintDialog printDialog(&printer);

    if (printDialog.exec() == QDialog::Accepted) {
        mWebBrowserWindowWidget->webView()->print(&printer);
    }
}

//==============================================================================

void WebBrowserWindowWindow::actionInspectTriggered()
{
    // Inspect the current page using our Web inspector

    mWebBrowserWindowWidget->showWebInspector();
}

//==============================================================================

void WebBrowserWindowWindow::actionReloadTriggered()
{
    // Reload the URL

    mWebBrowserWindowWidget->webView()->reload();
}

//==============================================================================

void WebBrowserWindowWindow::returnPressed()
{
    // Go to our home page (i.e. blank page), if the URL is empty, or load the
    // URL
    // Note: we enable/disable the progress bar based on whether the URL is our
    //       homepage since we don't want to see its progress in the latter
    //       case...

    QString url = mUrlValue->text().isEmpty()?
                      mWebBrowserWindowWidget->homePage():
                      mUrlValue->text();

    mWebBrowserWindowWidget->progressBarWidget()->setEnabled(url != mWebBrowserWindowWidget->homePage());

    mWebBrowserWindowWidget->webView()->load(url);
}

//==============================================================================

void WebBrowserWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

} // namespace WebBrowserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
