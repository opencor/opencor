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
// COMBINE support plugin
//==============================================================================

#include "combinefilemanager.h"
#include "combinesupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

PLUGININFO_FUNC COMBINESupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to support <a href=\"http://co.mbine.org/\">COMBINE</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter <a href=\"http://co.mbine.org/\">COMBINE</a>."));

    return new PluginInfo(PluginInfo::Support, false, false,
                          QStringList() << "StandardSupport" << "ZIPSupport",
                          descriptions);
}

//==============================================================================
// File interface
//==============================================================================

bool COMBINESupportPlugin::isFile(const QString &pFileName) const
{
    // Return whether the given file is of the type that we support

    return CombineFileManager::instance()->isFile(pFileName);
}

//==============================================================================

QString COMBINESupportPlugin::mimeType() const
{
    // Return the MIME type we support

    return CombineMimeType;
}

//==============================================================================

QString COMBINESupportPlugin::fileExtension() const
{
    // Return the extension of the type of file we support

    return CombineFileExtension;
}

//==============================================================================

QString COMBINESupportPlugin::fileTypeDescription() const
{
    // Return the description of the type of file we support

    return tr("COMBINE Archive");
}

//==============================================================================

QStringList COMBINESupportPlugin::fileTypeDefaultViews() const
{
    // Return the default views to use for the type of file we support

    return QStringList() << "SingleCellView";
}

//==============================================================================
// I18n interface
//==============================================================================

void COMBINESupportPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Plugin interface
//==============================================================================

bool COMBINESupportPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool COMBINESupportPlugin::pluginInterfacesOk(const QString &pFileName,
                                              QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void COMBINESupportPlugin::initializePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // Make a call to the instance of the COMBINE file manager so that it gets
    // properly set up (and therefore can start managing COMBINE archives)
    // before it actually gets used by other plugins
    // Note: we do it here rather than in initializePlugin() since we need the
    //       Core plugin to be initialised (so we can get access to our 'global'
    //       file manager)...

    CombineFileManager::instance();
}

//==============================================================================

void COMBINESupportPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void COMBINESupportPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
