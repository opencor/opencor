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
// File Browser window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

namespace Ui {
    class FileBrowserWindowWindow;
} // namespace Ui

//==============================================================================

class QMenu;
class QModelIndex;

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

class FileBrowserWindowWidget;

//==============================================================================

class FileBrowserWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit FileBrowserWindowWindow(QWidget *pParent);
    ~FileBrowserWindowWindow() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

private:
    Ui::FileBrowserWindowWindow *mGui;

    FileBrowserWindowWidget *mFileBrowserWindowWidget;

    QMenu *mContextMenu;

private slots:
    void actionHomeTriggered();
    void actionParentTriggered();
    void actionPreviousTriggered();
    void actionNextTriggered();

    void showCustomContextMenu() const;
    void itemDoubleClicked();
};

//==============================================================================

} // namespace FileBrowserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
