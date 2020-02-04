/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

    return new PluginInfo(PluginInfo::Category::Sample, true, false,
                          { "Core" },
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool SampleViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SampleViewPlugin::saveFile(const QString &pOldFileName,
                                const QString &pNewFileName,
                                bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SampleViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has had its permissions changed, so update our view
    // widget, if needed

    if (pFileName == mFileName) {
        mViewWidget->update(pFileName);
    }
}

//==============================================================================

void SampleViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so update our view widget, if needed

    if (pFileName == mFileName) {
        mViewWidget->update(pFileName);
    }
}

//==============================================================================

void SampleViewPlugin::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName)

    // The given file has been renamed, so update our view widget, if needed

    if (pOldFileName == mFileName) {
        mFileName = pNewFileName;

        mViewWidget->update(pNewFileName);
    }
}

//==============================================================================

void SampleViewPlugin::fileClosed(const QString &pFileName)
{
    // The given file has been closed, so update our internals, if needed

    if (pFileName == mFileName) {
        mFileName = QString();
    }
}

//==============================================================================
// I18n interface
//==============================================================================

void SampleViewPlugin::retranslateUi()
{
    // Retranslate our view widget, if needed

    if (!mFileName.isEmpty()) {
        mViewWidget->retranslateUi();
    }
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
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

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
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void SampleViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl)

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode SampleViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Mode::Sample;
}

//==============================================================================

QStringList SampleViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support, i.e. any in our case

    return {};
}

//==============================================================================

QString SampleViewPlugin::viewMimeType(const QString &pFileName) const
{
    Q_UNUSED(pFileName)

    // Return the MIME type for the given file

    return {};
}

//==============================================================================

QString SampleViewPlugin::viewDefaultFileExtension() const
{
    // Return the default file extension we support

    return {};
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
    Q_UNUSED(pFileName)

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
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return {};
}

//==============================================================================

} // namespace SampleView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
