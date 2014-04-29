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
// HelpWindow plugin
//==============================================================================

#include "helpwindowplugin.h"
#include "helpwindowwindow.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

PLUGININFO_FUNC HelpWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide help."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour fournir de l'aide."));

    return new PluginInfo(PluginInfo::Miscellaneous, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// GUI interface
//==============================================================================

void HelpWindowPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool HelpWindowPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * HelpWindowPlugin::viewWidget(const QString &pFileName,
                                       const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void HelpWindowPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString HelpWindowPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon HelpWindowPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool HelpWindowPlugin::saveFile(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void HelpWindowPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::fileModified(const QString &pFileName,
                                    const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::fileRenamed(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool HelpWindowPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void HelpWindowPlugin::retranslateUi()
{
    // Retranslate our help action

    retranslateAction(mHelpAction, tr("Help"),
                      tr("Show/hide the OpenCOR help"));

    // Retranslate our help window

    mHelpWindow->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void HelpWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our help window

    mHelpAction = newAction(true, QIcon(":/oxygen/apps/help-browser.png"),
                            Qt::Key_F1, mMainWindow);

    // Create our help window

    mHelpWindow = new HelpWindowWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::RightDockWidgetArea, mHelpWindow, mHelpAction);
}

//==============================================================================

void HelpWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::pluginInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our help window settings

    pSettings->beginGroup(mHelpWindow->objectName());
        mHelpWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void HelpWindowPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void HelpWindowPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
