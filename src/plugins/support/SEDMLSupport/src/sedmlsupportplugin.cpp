/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// SEDMLSupport plugin
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "sedmlfilemanager.h"
#include "sedmlsupportplugin.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

PLUGININFO_FUNC SEDMLSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support <a href=\"http://www.sed-ml.org/\">SED-ML</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter <a href=\"http://www.sed-ml.org/\">SED-ML</a>."));

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "Core" << "SEDMLAPI" << "StandardSupport",
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

void SEDMLSupportPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    Q_UNUSED(pMainWindow);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLSupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SEDMLSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // Make a call to the instance of the SED-ML file manager so that it gets
    // properly set up (and therefore can start managing SED-ML files) before it
    // actually gets used by other plugins
    // Note: we do it here rather than in initialize() since we need the Core
    //       plugin to be initialised (so we can get access to our 'global' file
    //       manager)...

    SedmlFileManager::instance();
}

//==============================================================================

void SEDMLSupportPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLSupportPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SEDMLSupportPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
