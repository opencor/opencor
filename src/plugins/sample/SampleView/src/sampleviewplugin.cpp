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
// Sample view plugin
//==============================================================================

#include "coreguiutils.h"
#include "sampleviewplugin.h"
#include "sampleviewwidget.h"

//==============================================================================

#include <QIcon>
#include <QMainWindow>

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
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

SampleViewPlugin::SampleViewPlugin() :
    mFileName(QString())
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool SampleViewPlugin::saveFile(const QString &pOldFileName,
                                const QString &pNewFileName,
                                bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);
    Q_UNUSED(pNeedFeedback);

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
    // The given file has had its permissions changed, so update our view
    // widget, if needed

    if (!pFileName.compare(mFileName))
        mViewWidget->update(pFileName);
}

//==============================================================================

void SampleViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileReloaded(const QString &pFileName,
                                    const bool &pFileChanged)
{
    // The given file has been reloaded, so update our view widget, if needed

    if (pFileChanged && !pFileName.compare(mFileName))
        mViewWidget->update(pFileName);
}

//==============================================================================

void SampleViewPlugin::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);

    // The given file has been renamed, so update our view widget, if needed

    if (!pOldFileName.compare(mFileName)) {
        mFileName = pNewFileName;

        mViewWidget->update(pNewFileName);
    }
}

//==============================================================================

void SampleViewPlugin::fileClosed(const QString &pFileName)
{
    // The given file has been closed, so update our internals, if needed

    if (!pFileName.compare(mFileName))
        mFileName = QString();
}

//==============================================================================
// I18n interface
//==============================================================================

void SampleViewPlugin::retranslateUi()
{
    // Retranslate our view widget, if needed

    if (!mFileName.isEmpty())
        mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool SampleViewPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SampleViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                          QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SampleViewPlugin::initializePlugin()
{
    // Create our Sample view widget

    mViewWidget = new SampleViewWidget(Core::mainWindow());

    // Hide our Sample view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void SampleViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
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

void SampleViewPlugin::handleUrl(const QUrl &pUrl)
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

    return SampleMode;
}

//==============================================================================

QStringList SampleViewPlugin::viewMimeTypes(const MimeTypeMode &pMimeTypeMode) const
{
    Q_UNUSED(pMimeTypeMode);

    // Return the MIME types we support, i.e. any in our case

    return QStringList();
}

//==============================================================================

QString SampleViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return QString();
}

//==============================================================================

QWidget * SampleViewPlugin::viewWidget(const QString &pFileName)
{
    // Update and return our Sample view widget using the given file

    mFileName = pFileName;

    mViewWidget->update(pFileName);

    return mViewWidget;
}

//==============================================================================

void SampleViewPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // Reset our internals

    mFileName = QString();
}

//==============================================================================

QString SampleViewPlugin::viewName() const
{
    // Return our Sample view's name

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
