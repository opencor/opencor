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
// Web browser window widget
//==============================================================================

#include "webbrowserwindowwidget.h"

//==============================================================================

#include <QWebHistory>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

WebBrowserWindowWidget::WebBrowserWindowWidget(QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent)
{
}

//==============================================================================

bool WebBrowserWindowWidget::isUrlSchemeSupported(const QString &pUrlScheme)
{
    // We support all URL schemes, but the OpenCOR one (opencor://...), which we
    // want to be handled (i.e. opened) the default way

    return pUrlScheme.compare("opencor");
}

//==============================================================================

void WebBrowserWindowWidget::clear()
{
    // Go to our home page and clear our history

    goToHomePage();

    history()->clear();
}

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
