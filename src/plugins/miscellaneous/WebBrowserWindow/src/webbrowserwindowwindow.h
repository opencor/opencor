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

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

class QWebView;

//==============================================================================

namespace Ui {
    class WebBrowserWindowWindow;
}

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

class WebBrowserWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit WebBrowserWindowWindow(QWidget *pParent);
    ~WebBrowserWindowWindow();

    virtual void retranslateUi();
protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::WebBrowserWindowWindow *mGui;

    QWebView *mWebBrowserWidget;
};

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
