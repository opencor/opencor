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

class QLineEdit;
class QMenu;

//==============================================================================

namespace Ui {
    class WebBrowserWindowWindow;
}

//==============================================================================

namespace OpenCOR {
namespace WebBrowserWindow {

//==============================================================================

class WebBrowserWindowWidget;

//==============================================================================

class WebBrowserWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit WebBrowserWindowWindow(QWidget *pParent);
    ~WebBrowserWindowWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::WebBrowserWindowWindow *mGui;

    WebBrowserWindowWidget *mWebBrowserWindowWidget;

    QMenu *mContextMenu;

    QLineEdit *mUrlValue;

private slots:
    void on_actionClear_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void on_actionCopy_triggered();
    void on_actionNormalSize_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionPrint_triggered();
    void on_actionInspect_triggered();
    void on_actionReload_triggered();

    void returnPressed();

    void urlChanged(const QUrl &pUrl);
    void showCustomContextMenu() const;
};

//==============================================================================

}   // namespace WebBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
