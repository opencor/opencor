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
} // namespace Ui

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
    ~WebBrowserWindowWindow() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

private:
    Ui::WebBrowserWindowWindow *mGui;

    WebBrowserWindowWidget *mWebBrowserWindowWidget;

    QMenu *mContextMenu;

    QLineEdit *mUrlValue;

private slots:
    void actionClearTriggered();
    void actionBackTriggered();
    void actionForwardTriggered();
    void actionCopyTriggered();
    void actionNormalSizeTriggered();
    void actionZoomInTriggered();
    void actionZoomOutTriggered();
    void actionPrintTriggered();
    void actionInspectTriggered();
    void actionReloadTriggered();

    void returnPressed();

    void urlChanged(const QUrl &pUrl);
    void showCustomContextMenu() const;
};

//==============================================================================

} // namespace WebBrowserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
