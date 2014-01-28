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
// FileBrowser plugin
//==============================================================================

#include "filebrowserplugin.h"
#include "filebrowserwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access your local files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à vos fichiers locaux."));

    return new PluginInfo(PluginInfo::Organisation, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void FileBrowserPlugin::initialize()
{
    // Create an action to show/hide our file browser window

    mFileBrowserAction = newAction(mMainWindow, true);

    // Create our file browser window

    mFileBrowserWindow = new FileBrowserWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileBrowserWindow,
                            GuiWindowSettings::Organisation,
                            mFileBrowserAction);
}

//==============================================================================

void FileBrowserPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file browser window settings

    loadWindowSettings(pSettings, mFileBrowserWindow);
}

//==============================================================================

void FileBrowserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file browser window settings

    saveWindowSettings(pSettings, mFileBrowserWindow);
}

//==============================================================================

void FileBrowserPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void FileBrowserPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool FileBrowserPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * FileBrowserPlugin::viewWidget(const QString &pFileName,
                                        const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void FileBrowserPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString FileBrowserPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon FileBrowserPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool FileBrowserPlugin::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileBrowserPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::fileModified(const QString &pFileName,
                                     const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool FileBrowserPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void FileBrowserPlugin::retranslateUi()
{
    // Retranslate our file browser action

    retranslateAction(mFileBrowserAction, tr("File Browser"),
                      tr("Show/hide the File Browser window"));

    // Retranslate our file browser window

    mFileBrowserWindow->retranslateUi();
}

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
