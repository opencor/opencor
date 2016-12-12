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
// Python Qt Support plugin
//==============================================================================

#include "corecliutils.h"
#include "pythonqtsupportplugin.h"

//==============================================================================

#include "ctkAbstractPythonManager.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace PythonQtSupport {

//==============================================================================

PLUGININFO_FUNC PythonQtSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the Python Qt support plugin."));
    descriptions.insert("fr", QString::fromUtf8("the Python Qt support plugin."));

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "PythonQtAPI",
                          descriptions);
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonQtSupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonQtSupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                               QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonQtSupportPlugin::initializePlugin()
{
    // Create and initialise a new CTK Python manager

    auto pythonManager = new ctkAbstractPythonManager(this);
    pythonManager->initialize();

    // Save it in our instance

    instance()->mPythonManager = pythonManager;
}

//==============================================================================

void PythonQtSupportPlugin::finalizePlugin()
{
    delete mPythonManager;
}

//==============================================================================

void PythonQtSupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonQtSupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
//==============================================================================

ctkAbstractPythonManager *PythonQtSupportPlugin::pythonManager(void)
{
    return instance()->mPythonManager;
}

//==============================================================================

PythonQtSupportPlugin *PythonQtSupportPlugin::instance(void)
{
    // Return the 'global' instance of our Python Qt plugin

    static PythonQtSupportPlugin pluginInstance;
    return static_cast<PythonQtSupportPlugin *>(Core::globalInstance("OpenCOR::PythonQtSupport::PythonQtSupportPlugin",
                                                              &pluginInstance));
}

//==============================================================================

}   // namespace PythonQtSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
