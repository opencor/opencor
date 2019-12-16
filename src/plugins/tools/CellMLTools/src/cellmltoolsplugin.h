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
// CellML tools plugin
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "cliinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo();

//==============================================================================

class CellMLToolsPlugin : public QObject, public CliInterface,
                          public GuiInterface, public I18nInterface,
                          public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CellMLToolsPlugin" FILE "cellmltoolsplugin.json")

    Q_INTERFACES(OpenCOR::CliInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
#include "cliinterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    enum class Command {
        Export,
        Validate
    };

    QString mFileName;

    QMenu *mCellmlFileExportToMenu = nullptr;

    QAction *mExportToCellml10Action = nullptr;

    QAction *mExportToUserDefinedFormatAction = nullptr;

    void exportTo(CellMLSupport::CellmlFile::Version pVersion);

    void runHelpCommand();
    bool runExportCommand(const QStringList &pArguments);
    bool runValidateCommand(const QStringList &pArguments);

    bool runCommand(Command pCommand, const QStringList &pArguments);

private slots:
    void exportToCellml10();

    void exportToUserDefinedFormat();
};

//==============================================================================

} // namespace CellMLTools
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
