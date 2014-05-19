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
// SampleView plugin
//==============================================================================

#include "sampleviewplugin.h"

//==============================================================================

#include <QIcon>

//==============================================================================

namespace OpenCOR {
namespace SampleView {

//==============================================================================

PLUGININFO_FUNC SampleViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides a test view."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit une vue de test."));

    return new PluginInfo(PluginInfo::Sample, true, false,
                          QStringList() << "Core" << "Sample",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool SampleViewPlugin::saveFile(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SampleViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void SampleViewPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================
// Plugin interface
//==============================================================================

void SampleViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    Q_UNUSED(pMainWindow);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::pluginInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode SampleViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Sample;
}

//==============================================================================

QStringList SampleViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support
    // Note: we allow any kind of file, hence our empty string list...

    return QStringList();
}

//==============================================================================

void SampleViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * SampleViewPlugin::viewWidget(const QString &pFileName,
                                       const bool &pCreate)
{
    // We don't handle this interface...

    return 0;
}

//==============================================================================

void SampleViewPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString SampleViewPlugin::viewName() const
{
    // Return our raw view's name

    return tr("Sample");
}

//==============================================================================

QIcon SampleViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

}   // namespace SampleView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
