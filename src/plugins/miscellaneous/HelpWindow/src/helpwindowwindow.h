/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Help window
//==============================================================================

#pragma once

//==============================================================================

#include "windowwidget.h"

//==============================================================================

class QMenu;
class QUrl;

//==============================================================================

namespace Ui {
    class HelpWindowWindow;
} // namespace Ui

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

class HelpWindowWidget;

//==============================================================================

class HelpWindowWindow : public Core::WindowWidget
{
    Q_OBJECT

public:
    explicit HelpWindowWindow(QWidget *pParent);
    ~HelpWindowWindow() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

private:
    Ui::HelpWindowWindow *mGui;

    HelpWindowWidget *mHelpWindowWidget;

    QMenu *mContextMenu;

private slots:
    void actionHomeTriggered();
    void actionBackTriggered();
    void actionForwardTriggered();
    void actionCopyTriggered();
    void actionNormalSizeTriggered();
    void actionZoomInTriggered();
    void actionZoomOutTriggered();
    void actionPrintTriggered();

    void showCustomContextMenu() const;
};

//==============================================================================

} // namespace HelpWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
