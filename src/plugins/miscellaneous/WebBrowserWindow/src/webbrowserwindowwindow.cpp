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
// Web Browser window
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"
#include "toolbarwidget.h"
#include "webbrowserwindowwindow.h"

//==============================================================================

#include "ui_webbrowserwindowwindow.h"

//==============================================================================

#include <QClipboard>
#include <QMenu>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QWebFrame>
#include <QWebHistory>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

enum {
    MinimumZoomLevel =  1,
    DefaultZoomLevel = 10
};

//==============================================================================

WebBrowserWindowWindow::WebBrowserWindowWindow(QWidget *pParent) :
    Core::WindowWidget(pParent),
    mGui(new Ui::WebBrowserWindowWindow),
    mZoomLevel(-1)   // This will ensure that mZoomLevel gets initialised by our
                     // first call to setZoomLevel
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->urlValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our URL value widget...
#endif

    // Make the URL value our focus proxy

    setFocusProxy(mGui->urlValue);

    // Initially, we cannot go backward/forward

    mGui->actionBack->setEnabled(false);
    mGui->actionForward->setEnabled(false);

    // Create a tool bar widget with different buttons

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);

    toolBarWidget->addAction(mGui->actionClear);
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

    mGui->dockWidgetContents->layout()->addWidget(Core::newLineWidget(this));
    mGui->dockWidgetContents->layout()->addWidget(toolBarWidget);

    // Create and add the web browser widget

    mWebBrowserWidget = new QWebView(this);

    setZoomLevel(DefaultZoomLevel);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWebBrowserWidget,
                                                                           true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWebBrowserWidget,
                                                                           true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Some connections to handle the change in URL or page action status of our
    // web browser widget

    connect(mWebBrowserWidget, SIGNAL(urlChanged(const QUrl &)),
            this, SLOT(urlChanged(const QUrl &)));

    connect(mWebBrowserWidget->pageAction(QWebPage::Back), SIGNAL(changed()),
            this, SLOT(documentChanged()));
    connect(mWebBrowserWidget->pageAction(QWebPage::Forward), SIGNAL(changed()),
            this, SLOT(documentChanged()));

    // Start with a clear web browser widget

    on_actionClear_triggered();

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

    // We want our own context menu for the help window widget (indeed, we don't
    // want the default one, which has the reload menu item and not the other
    // actions that we have in our tool bar widget)

    mWebBrowserWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mWebBrowserWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu()));

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
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
    // Retranslate the whole window

    mGui->retranslateUi(this);
}

//==============================================================================

void WebBrowserWindowWindow::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::WindowWidget::resizeEvent(pEvent);
}

//==============================================================================

void WebBrowserWindowWindow::updateActions()
{
    // Update the enabled state of our various actions

    mGui->actionClear->setEnabled(!mGui->urlValue->text().isEmpty());

    mGui->actionCopy->setEnabled(false);

    mGui->actionNormalSize->setEnabled(mZoomLevel != DefaultZoomLevel);
    mGui->actionZoomOut->setEnabled(mZoomLevel != MinimumZoomLevel);

    mGui->actionPrint->setEnabled(QPrinterInfo::availablePrinterNames().count());
}

//==============================================================================

void WebBrowserWindowWindow::urlChanged(const QUrl &pUrl)
{
    // The URL has changed, so update our URL value

    QString url = pUrl.toString();

    mGui->urlValue->setText(url.compare("about:blank")?pUrl.toString():QString());

    updateActions();
}

//==============================================================================

void WebBrowserWindowWindow::documentChanged()
{
    // A new page has been loaded, resulting in the previous or next page
    // becoming either available or not

    QAction *action = qobject_cast<QAction *>(sender());

    if (action == mWebBrowserWidget->pageAction(QWebPage::Back))
        mGui->actionBack->setEnabled(action->isEnabled());
    else if (action == mWebBrowserWidget->pageAction(QWebPage::Forward))
        mGui->actionForward->setEnabled(action->isEnabled());
}

//==============================================================================

void WebBrowserWindowWindow::setZoomLevel(const int &pZoomLevel)
{
    if (pZoomLevel == mZoomLevel)
        return;

    // Set the zoom level of the help document contents to a particular value

    mZoomLevel = pZoomLevel;

    mWebBrowserWidget->setZoomFactor(0.1*mZoomLevel);

    // Emit a few zoom-related signals

    updateActions();
}

//==============================================================================

void WebBrowserWindowWindow::on_urlValue_returnPressed()
{
    // Load the URL

    mWebBrowserWidget->load(mGui->urlValue->text());

    updateActions();
}

//==============================================================================

void WebBrowserWindowWindow::on_refreshButton_clicked()
{
    // Reload the URL

    mWebBrowserWidget->reload();
}

//==============================================================================

void WebBrowserWindowWindow::on_actionClear_triggered()
{
    // Go to the home page, i.e. a blank page
    // Note: to set a blank page will make our web page completely white, which
    //       looks better than the default grey background...

    mGui->urlValue->setText("about:blank");

    on_urlValue_returnPressed();

    mWebBrowserWidget->history()->clear();
}

//==============================================================================

void WebBrowserWindowWindow::on_actionBack_triggered()
{
    // Go to the previous page

    mWebBrowserWidget->back();
}

//==============================================================================

void WebBrowserWindowWindow::on_actionForward_triggered()
{
    // Go to the next page

    mWebBrowserWidget->forward();
}

//==============================================================================

void WebBrowserWindowWindow::on_actionNormalSize_triggered()
{
    // Reset the zoom level of the page contents

    setZoomLevel(DefaultZoomLevel);
}

//==============================================================================

void WebBrowserWindowWindow::on_actionZoomIn_triggered()
{
    // Zoom in the page contents

    setZoomLevel(mZoomLevel+1);
}

//==============================================================================

void WebBrowserWindowWindow::on_actionZoomOut_triggered()
{
    // Zoom out the page contents

    setZoomLevel(qMax(int(MinimumZoomLevel), mZoomLevel-1));
}

//==============================================================================

void WebBrowserWindowWindow::on_actionPrint_triggered()
{
    // Retrieve the printer with which the user wants to print the page and
    // print it, should s/he still want to go ahead with the printing

    QPrinter printer;
    QPrintDialog printDialog(&printer);

    if (printDialog.exec() == QDialog::Accepted)
        mWebBrowserWidget->print(&printer);
}

//==============================================================================

void WebBrowserWindowWindow::showCustomContextMenu() const
{
    // Show our context menu which items match the contents of our tool bar
    // widget

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
