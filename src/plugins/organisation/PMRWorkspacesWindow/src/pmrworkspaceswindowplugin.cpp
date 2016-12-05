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
// PMR Workspaces window plugin
//==============================================================================

#include "coreguiutils.h"
#include "pmrworkspaceswindowplugin.h"
#include "pmrworkspaceswindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PLUGININFO_FUNC PMRWorkspacesWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to manage a user's <a href=\"https://models.physiomeproject.org/\">PMR</a> workspaces."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder les espaces de travail <a href=\"https://models.physiomeproject.org/\">PMR</a> d'un utilisateur."));

    return new PluginInfo(PluginInfo::Organisation, true, false,
                          QStringList() << "PMRSupport" << "WebViewerWidget",
                          descriptions,
                          QStringList() << "FileBrowserWindow" << "FileOrganiserWindow");
}

//==============================================================================
// File handling interface
//==============================================================================

bool PMRWorkspacesWindowPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PMRWorkspacesWindowPlugin::saveFile(const QString &pOldFileName,
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

void PMRWorkspacesWindowPlugin::fileOpened(const QString &pFileName)
{
    // We don't handle this interface...

    Q_UNUSED(pFileName);
}

//==============================================================================

void PMRWorkspacesWindowPlugin::filePermissionsChanged(const QString &pFileName)
{
    // We don't handle this interface...

    Q_UNUSED(pFileName);
}

//==============================================================================

void PMRWorkspacesWindowPlugin::fileModified(const QString &pFileName)
{
    // We don't handle this interface...

    Q_UNUSED(pFileName);
}

//==============================================================================

void PMRWorkspacesWindowPlugin::fileReloaded(const QString &pFileName,
                                             const bool &pFileChanged)
{
    Q_UNUSED(pFileChanged);

    // The given file has been reloaded, so let its corresponding view window
    // know about it

    mPmrWorkspacesWindowWindow->fileReloaded(pFileName);
}

//==============================================================================

void PMRWorkspacesWindowPlugin::fileRenamed(const QString &pOldFileName,
                                            const QString &pNewFileName)
{
    // The given file has been renamed, so let our view window know about it

    mPmrWorkspacesWindowWindow->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void PMRWorkspacesWindowPlugin::fileClosed(const QString &pFileName)
{
    // We don't handle this interface...

    Q_UNUSED(pFileName);
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWorkspacesWindowPlugin::retranslateUi()
{
    // Retranslate our workspaces window action

    retranslateAction(mPmrWorkspacesWindowAction, tr("PMR Workspaces"),
                      tr("Show/hide the PMR Workspaces window"));

    // Retranslate our workspaces window

    mPmrWorkspacesWindowWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool PMRWorkspacesWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool PMRWorkspacesWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                                   QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void PMRWorkspacesWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our workspaces window

    mPmrWorkspacesWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our workspaces window

    mPmrWorkspacesWindowWindow = new PmrWorkspacesWindowWindow(Core::mainWindow());
}

//==============================================================================

void PMRWorkspacesWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindowWindow->objectName());
        mPmrWorkspacesWindowWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindowWindow->objectName());
        mPmrWorkspacesWindowWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PMRWorkspacesWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * PMRWorkspacesWindowPlugin::windowAction() const
{
    // Return our window action

    return mPmrWorkspacesWindowAction;
}

//==============================================================================

QDockWidget * PMRWorkspacesWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mPmrWorkspacesWindowWindow;
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
