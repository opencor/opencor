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
// PMR Workspaces plugin
//==============================================================================

#include "coreguiutils.h"
#include "pmrworkspacesplugin.h"
#include "pmrworkspaceswindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PLUGININFO_FUNC PMRWorkspacesPluginInfo()
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

bool PMRWorkspacesPlugin::isIndirectRemoteFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    return false;
}

//==============================================================================

bool PMRWorkspacesPlugin::saveFile(const QString &pOldFileName,
                                   const QString &pNewFileName,
                                   bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback);

    return true;
}

//==============================================================================

void PMRWorkspacesPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)
}

//==============================================================================

void PMRWorkspacesPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName)
}

//==============================================================================

void PMRWorkspacesPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName)
}

//==============================================================================

void PMRWorkspacesPlugin::fileReloaded(const QString &pFileName,
                                       const bool &pFileChanged)
{
    Q_UNUSED(pFileChanged)

    // The given file has been reloaded, so let its corresponding view widget
    // know about it

    mPmrWorkspacesWindow->fileReloaded(pFileName);
}

//==============================================================================

void PMRWorkspacesPlugin::fileRenamed(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    // Let our view widget know that a file has been renamed

    mPmrWorkspacesWindow->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void PMRWorkspacesPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)
}

//==============================================================================
// I18n interface
//==============================================================================

void PMRWorkspacesPlugin::retranslateUi()
{
    // Retranslate our workspaces window action

    retranslateAction(mPmrWorkspacesAction, tr("PMR Workspaces"),
                      tr("Show/hide the PMR Workspaces window"));

    // Retranslate our workspaces window

    mPmrWorkspacesWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void PMRWorkspacesPlugin::initializePlugin()
{
    // Create an action to show/hide our workspaces window

    mPmrWorkspacesAction = Core::newAction(true, Core::mainWindow());

    // Create our workspaces window

    mPmrWorkspacesWindow = new PmrWorkspacesWindow(Core::mainWindow());
}

//==============================================================================

void PMRWorkspacesPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void PMRWorkspacesPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindow->objectName());
        mPmrWorkspacesWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our workspaces window settings

    pSettings->beginGroup(mPmrWorkspacesWindow->objectName());
        mPmrWorkspacesWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PMRWorkspacesPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea PMRWorkspacesPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::LeftDockWidgetArea;
}

//==============================================================================

QAction * PMRWorkspacesPlugin::windowAction() const
{
    // Return our window action

    return mPmrWorkspacesAction;
}

//==============================================================================

QDockWidget * PMRWorkspacesPlugin::windowWidget() const
{
    // Return our window widget

    return mPmrWorkspacesWindow;
}

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
