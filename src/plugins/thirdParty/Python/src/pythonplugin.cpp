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
// Python plugin
//==============================================================================

#include "pythonplugin.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Python {

//==============================================================================

PLUGININFO_FUNC PythonPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide the <a href=\"https://www.python.org/\">Python</a> language."));
    descriptions.insert("fr", QString::fromUtf8("une extension ..."));

    return new PluginInfo("Third-party", false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

void PythonPlugin::initializePlugin()
{
    // TODO: Initialise Python
}

//==============================================================================

void PythonPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // TODO: Retrieve our Python settings
}

//==============================================================================

void PythonPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // TODO: Keep track of our Python settings
}

//==============================================================================

void PythonPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace Python
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
