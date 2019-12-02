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
// File Organiser window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QAbstractItemModel>

//==============================================================================

namespace Ui {
    class FileOrganiserWindowWindow;
} // namespace Ui

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

class FileOrganiserWindowWidget;

//==============================================================================

class FileOrganiserWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit FileOrganiserWindowWindow(QWidget *pParent);
    ~FileOrganiserWindowWindow() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

private:
    Ui::FileOrganiserWindowWindow *mGui;

    FileOrganiserWindowWidget *mFileOrganiserWindowWidget;

    QMenu *mContextMenu;

private slots:
    void actionNewTriggered();
    void actionDeleteTriggered();

    void showCustomContextMenu() const;
    void itemDoubleClicked(const QModelIndex &pItemIndex);
};

//==============================================================================

} // namespace FileOrganiserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
