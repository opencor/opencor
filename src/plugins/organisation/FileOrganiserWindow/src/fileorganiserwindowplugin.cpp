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
// FileOrganiserWindow plugin
//==============================================================================

#include "fileorganiserwindowplugin.h"
#include "fileorganiserwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

PLUGININFO_FUNC FileOrganiserWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to virtually organise your files."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour organiser virtuellement vos fichiers."));

    return new PluginInfo(PluginInfo::Organisation, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// GUI interface
//==============================================================================

void FileOrganiserWindowPlugin::updateGui(Plugin *pViewPlugin,
                                          const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool FileOrganiserWindowPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * FileOrganiserWindowPlugin::viewWidget(const QString &pFileName,
                                                const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void FileOrganiserWindowPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString FileOrganiserWindowPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon FileOrganiserWindowPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool FileOrganiserWindowPlugin::saveFile(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void FileOrganiserWindowPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::fileModified(const QString &pFileName,
                                             const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::fileRenamed(const QString &pOldFileName,
                                            const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================
// I18n interface
//==============================================================================

void FileOrganiserWindowPlugin::retranslateUi()
{
    // Retranslate our file organiser action

    retranslateAction(mFileOrganiserAction, tr("File Organiser"),
                      tr("Show/hide the File Organiser window"));

    // Retranslate our file organiser window

    mFileOrganiserWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void FileOrganiserWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our file organiser window

    mFileOrganiserAction = newAction(true, mMainWindow);

    // Create our file organiser window

    mFileOrganiserWindow = new FileOrganiserWindowWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileOrganiserWindow,
                            mFileOrganiserAction);
}

//==============================================================================

void FileOrganiserWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::pluginInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void FileOrganiserWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file organiser window settings

    pSettings->beginGroup(mFileOrganiserWindow->objectName());
        mFileOrganiserWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file organiser window settings

    pSettings->beginGroup(mFileOrganiserWindow->objectName());
        mFileOrganiserWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void FileOrganiserWindowPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
