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
// SED-ML support plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filetypeinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

PLUGININFO_FUNC SEDMLSupportPluginInfo();

//==============================================================================

static const auto SedmlMimeType      = QStringLiteral("application/sedml+xml");
static const auto SedmlFileExtension = QStringLiteral("sedml");

//==============================================================================

class SEDMLSupportPlugin : public QObject, public FileTypeInterface,
                           public GuiInterface, public I18nInterface,
                           public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SEDMLSupportPlugin" FILE "sedmlsupportplugin.json")

    Q_INTERFACES(OpenCOR::FileTypeInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit SEDMLSupportPlugin();

#include "filetypeinterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    QAction *mFileNewSedmlFileAction;

private slots:
    void newSedmlFile();
};

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
