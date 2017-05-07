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
// File Browser window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

namespace Ui {
    class FileBrowserWindowWindow;
}

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
    ~FileBrowserWindowWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::FileBrowserWindowWindow *mGui;

    FileBrowserWindowWidget *mFileBrowserWindowWidget;

    QMenu *mContextMenu;

private slots:
    void on_actionHome_triggered();
    void on_actionParent_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();

    void showCustomContextMenu() const;
    void itemDoubleClicked();
};

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
