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

#pragma once

//==============================================================================

#include "webviewerwidget.h"

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

class WebBrowserWindowWidget : public WebViewerWidget::WebViewerWidget
{
    Q_OBJECT

public:
    explicit WebBrowserWindowWidget(QWidget *pParent);

    virtual bool isUrlSchemeSupported(const QString &pUrlScheme);

    void clear();
};

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
