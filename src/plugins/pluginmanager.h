/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

    Plugins plugins() const;
    Plugins loadedPlugins() const;

    QString pluginsDir() const;

    Plugin * plugin(const QString &pName) const;
    Plugin * corePlugin() const;

private:
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
