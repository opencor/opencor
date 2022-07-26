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
// BondGraphEditor window
//==============================================================================

#include "bondgrapheditorwindow.h"

#include <QDesktopServices>
#include <QLayout>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace BondGraphEditorWindow {

//==============================================================================

BondGraphEditorWindow::BondGraphEditorWindow(QWidget *p_parent) :
    Core::WindowWidget(p_parent),
    mGui(new BondGraphUIWidget(p_parent))
{
    setObjectName("BondGraphEditorWindow");
    setWidget(mGui);
    setWindowTitle(tr("Graphical BondGraph Editor"));
    //Open cellml file in CellML editor when exported by the user
    connect(mGui, &BondGraphUIWidget::cellMLFileGenerated,
            [this](QString path) {
                QDesktopServices::openUrl("opencor://openFile/" + path);
            });
}

//==============================================================================

BondGraphEditorWindow::~BondGraphEditorWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void BondGraphEditorWindow::loadSettings(QSettings &p_settings)
{
    // Retrieve the settings of our Web browser window widget

    mGui->readSettings(p_settings);
}

//==============================================================================

void BondGraphEditorWindow::saveSettings(QSettings &p_settings) const
{
    mGui->saveSettings(p_settings);
}

//==============================================================================

} // namespace BondGraphEditorWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
