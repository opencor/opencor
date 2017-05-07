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
