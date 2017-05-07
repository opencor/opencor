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
// Plugin manager
//==============================================================================

#pragma once

//==============================================================================

#include "coreinterface.h"
#include "plugin.h"
#include "plugininfo.h"

//==============================================================================

#include <QObject>

//==============================================================================

class QCoreApplication;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(const bool &pGuiMode = true);
    ~PluginManager();

    bool guiMode() const;

    Plugins plugins() const;
    Plugins sortedPlugins() const;

    Plugins loadedPlugins() const;
    Plugins sortedLoadedPlugins() const;

    QString pluginsDir() const;

    Plugin * plugin(const QString &pName) const;
    Plugin * corePlugin() const;

private:
    bool mGuiMode;

    QString mPluginsDir;

    Plugins mPlugins;
    Plugins mLoadedPlugins;

    Plugin *mCorePlugin;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
