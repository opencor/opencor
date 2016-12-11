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
// PythonQt plugin
//==============================================================================

#include "pythonqtplugin.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace PythonQt {

//==============================================================================

PLUGININFO_FUNC PythonQtPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to allow the use of Qt with Python."));
    descriptions.insert("fr", QString::fromUtf8("une extension ..."));

    return new PluginInfo(PluginInfo::ThirdParty, false, false,
                          QStringList() << "Python",
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonQtPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonQtPlugin::pluginInterfacesOk(const QString &pFileName,
                                        QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================
void PythonQtPlugin::initializePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonQtPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonQtPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // TODO: Retrieve our Python Qt settings
}

//==============================================================================

void PythonQtPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // TODO: Keep track of our Python Qt settings
}

//==============================================================================

void PythonQtPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace PythonQt
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
