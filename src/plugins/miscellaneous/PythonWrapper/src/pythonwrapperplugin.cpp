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
// Python wrapper plugin
//==============================================================================

#include "corecliutils.h"
#include "pythonwrappercore.h"
#include "pythonwrapperdatastore.h"
#include "pythonwrapperplugin.h"
#include "pythonwrappersinglecellview.h"
#include "singlecellviewplugin.h"

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

PLUGININFO_FUNC PythonWrapperPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access OpenCOR data from <a href=\"http://www.python.org/\">Python</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder... <a href=\"http://www.python.org/\">Python</a>."));

    return new PluginInfo(PluginInfo::Tools, true, false,
                          QStringList() << "PythonQtSupport" << "SingleCellView",
                          descriptions);
}

//==============================================================================

PythonWrapperPlugin::PythonWrapperPlugin()
{
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PythonWrapperPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PythonWrapperPlugin::pluginInterfacesOk(const QString &pFileName,
                                             QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PythonWrapperPlugin::initializePlugin()
{
    // Create a Python module to access OpenCOR's objects

    mOpenCORModule = PythonQt::self()->createModuleFromScript("OpenCOR");

    mPythonWrapperCore = new PythonWrapperCore(mOpenCORModule);
    mPythonWrapperDataStore = new PythonWrapperDataStore(mOpenCORModule);
    mPythonWrapperSingleCellView = new PythonWrapperSingleCellView(mOpenCORModule);
}

//==============================================================================

void PythonWrapperPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Look for simulation views

    foreach (Plugin *plugin, pLoadedPlugins) {

        if (!plugin->name().compare("SingleCellView")) {

            auto *singleCellViewPlugin = qobject_cast<SingleCellView::SingleCellViewPlugin *>(plugin->instance());
            instance()->mSingleCellViewWidget = singleCellViewPlugin->viewWidget();

            break;
        }
    }
}

//==============================================================================

void PythonWrapperPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void PythonWrapperPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Our own methods
//==============================================================================

PythonWrapperPlugin *PythonWrapperPlugin::instance(void)
{
    // Return the 'global' instance of our Python wrapper plugin

    static PythonWrapperPlugin pluginInstance;
    return static_cast<PythonWrapperPlugin *>(Core::globalInstance("OpenCOR::PythonQtSupport::PythonWrapperPlugin",
                                                                   &pluginInstance));
}

//==============================================================================

SingleCellView::SingleCellViewWidget *PythonWrapperPlugin::singleCellViewWidget()
{
    return mSingleCellViewWidget;
}

//==============================================================================

}   // namespace PythonSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
