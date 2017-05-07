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
// PMR Workspaces window plugin
//==============================================================================

#pragma once

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"
#include "preferencesinterface.h"
#include "windowinterface.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PLUGININFO_FUNC PMRWorkspacesWindowPluginInfo();

//==============================================================================

class PmrWorkspacesWindowWindow;

//==============================================================================

class PMRWorkspacesWindowPlugin : public QObject, public I18nInterface,
                                  public PluginInterface,
                                  public PreferencesInterface,
                                  public WindowInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PMRWorkspacesWindowPlugin" FILE "pmrworkspaceswindowplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::PreferencesInterface)
    Q_INTERFACES(OpenCOR::WindowInterface)

public:
#include "i18ninterface.inl"
#include "plugininterface.inl"
#include "preferencesinterface.inl"
#include "windowinterface.inl"

private:
    QAction *mPmrWorkspacesWindowAction;

    PmrWorkspacesWindowWindow *mPmrWorkspacesWindowWindow;
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
