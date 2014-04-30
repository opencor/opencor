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
// FileBrowserWindow plugin
//==============================================================================

#include "filebrowserwindowplugin.h"
#include "filebrowserwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

PLUGININFO_FUNC FileBrowserWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access your local files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à vos fichiers locaux."));

    return new PluginInfo(PluginInfo::Organisation, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// GUI interface
//==============================================================================

void FileBrowserWindowPlugin::updateGui(Plugin *pViewPlugin,
                                        const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool FileBrowserWindowPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * FileBrowserWindowPlugin::viewWidget(const QString &pFileName,
                                              const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void FileBrowserWindowPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString FileBrowserWindowPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon FileBrowserWindowPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool FileBrowserWindowPlugin::saveFile(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileBrowserWindowPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::fileModified(const QString &pFileName,
                                           const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::fileRenamed(const QString &pOldFileName,
                                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void FileBrowserWindowPlugin::retranslateUi()
{
    // Retranslate our file browser action

    retranslateAction(mFileBrowserAction, tr("File Browser"),
                      tr("Show/hide the File Browser window"));

    // Retranslate our file browser window

    mFileBrowserWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void FileBrowserWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our file browser window

    mFileBrowserAction = newAction(true, mMainWindow);

    // Create our file browser window

    mFileBrowserWindow = new FileBrowserWindowWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileBrowserWindow,
                            mFileBrowserAction);
}

//==============================================================================

void FileBrowserWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::pluginInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileBrowserWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file browser window settings

    pSettings->beginGroup(mFileBrowserWindow->objectName());
        mFileBrowserWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file browser window settings

    pSettings->beginGroup(mFileBrowserWindow->objectName());
        mFileBrowserWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileBrowserWindowPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
