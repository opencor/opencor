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
// Web browser window widget
//==============================================================================

#include "webbrowserwindowwidget.h"

//==============================================================================

#include <QWebHistory>
#include <QWebView>

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

WebBrowserWindowWidget::WebBrowserWindowWidget(QWidget *pParent) :
    WebViewerWidget::WebViewerWidget(pParent)
{
    // Go to our home page

    goToHomePage();
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

    webView()->history()->clear();
}

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
