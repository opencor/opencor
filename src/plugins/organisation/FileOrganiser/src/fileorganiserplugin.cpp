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
// FileOrganiser plugin
//==============================================================================

#include "fileorganiserplugin.h"
#include "fileorganiserwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to virtually organise your files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour organiser virtuellement vos fichiers."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Organisation,
                          false,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void FileOrganiserPlugin::initialize()
{
    // Create an action to show/hide our file organiser window

    mFileOrganiserAction = newAction(mMainWindow, true);

    // Create our file organiser window

    mFileOrganiserWindow = new FileOrganiserWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileOrganiserWindow,
                            GuiWindowSettings::Organisation,
                            mFileOrganiserAction);
}

//==============================================================================

void FileOrganiserPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file organiser window settings

    loadWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file organiser window settings

    saveWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::runCliCommand(const QString &pCommand,
                                        const QStringList &pArguments,
                                        int *pRes)
{
    Q_UNUSED(pCommand);
    Q_UNUSED(pArguments);

    // We don't handle this interface...

    *pRes = 0;
}

//==============================================================================
// GUI interface
//==============================================================================

void FileOrganiserPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::updateGui(Plugin *pViewPlugin,
                                    const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * FileOrganiserPlugin::viewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

QWidget * FileOrganiserPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

QString FileOrganiserPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon FileOrganiserPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool FileOrganiserPlugin::saveFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileOrganiserPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::fileRenamed(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool FileOrganiserPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void FileOrganiserPlugin::retranslateUi()
{
    // Retranslate our file organiser action

    retranslateAction(mFileOrganiserAction, tr("File Organiser"),
                      tr("Show/hide the File Organiser window"));

    // Retranslate our file organiser window

    mFileOrganiserWindow->retranslateUi();
}

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
