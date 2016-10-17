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
#include "webbrowserwindowwindow.h"

//==============================================================================

#include "ui_webbrowserwindowwindow.h"

//==============================================================================

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

#ifdef Q_OS_MAC
    mGui->urlValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our URL value widget...
#endif

    // Make the URL value our focus proxy

    setFocusProxy(mGui->urlValue);

    // Create and add the web browser widget
    // Note: to set an empty page to our web browser widget will make it
    //       completely, which looks better than the default grey background...

    mWebBrowserWidget = new QWebView(this);

    mWebBrowserWidget->setHtml(QString());

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWebBrowserWidget,
                                                                           true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mWebBrowserWidget,
                                                                           true, false, false, false));
#else
    #error Unsupported platform
#endif

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

void WebBrowserWindowWindow::on_urlValue_returnPressed()
{
    // Load the URL

    mWebBrowserWidget->load(QUrl(mGui->urlValue->text()));
}

//==============================================================================

void WebBrowserWindowWindow::on_refreshButton_clicked()
{
    // Reload the URL

    mWebBrowserWidget->reload();
}

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
