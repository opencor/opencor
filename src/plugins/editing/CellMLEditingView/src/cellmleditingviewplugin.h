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
// CellML Editing view plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class CellmlEditingViewInterface;

//==============================================================================

namespace EditorWidget {
    class EditorWidget;
} // namespace EditorWidget

//==============================================================================

namespace CellMLEditingView {

//==============================================================================

PLUGININFO_FUNC CellMLEditingViewPluginInfo();

//==============================================================================

class CellMLEditingViewPlugin : public QObject, public FileHandlingInterface,
                                public GuiInterface, public I18nInterface,
                                public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CellMLEditingViewPlugin" FILE "cellmleditingviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QAction *mEditReformatAction = nullptr;
    QAction *mEditReformatSeparator = nullptr;

    QAction *mToolsCellmlValidationAction = nullptr;
    QAction *mToolsCellmlValidationSeparator = nullptr;

    QString mFileName;

    EditorWidget::EditorWidget *mEditor = nullptr;

    CellmlEditingViewInterface *mCellmlEditingViewInterface = nullptr;

private slots:
    void reformat();

    void cellmlValidation();
};

//==============================================================================

} // namespace CellMLEditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
