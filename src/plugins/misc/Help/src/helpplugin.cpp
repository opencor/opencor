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
// Help plugin
//==============================================================================

#include "helpplugin.h"
#include "helpwindow.h"

//==============================================================================

#include <QAction>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

PLUGININFO_FUNC HelpPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to provide help."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour fournir de l'aide."));

    return new PluginInfo(PluginInfo::Miscellaneous, true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void HelpPlugin::initialize()
{
    // Create an action to show/hide our help window

    mHelpAction = newAction(mMainWindow, true,
                            ":/oxygen/apps/help-browser.png",
                            Qt::Key_F1);

    // Create our help window

    mHelpWindow = new HelpWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::RightDockWidgetArea, mHelpWindow,
                            GuiWindowSettings::Help, mHelpAction);
}

//==============================================================================

void HelpPlugin::finalize()
{
}

//==============================================================================

void HelpPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our help window settings

    loadWindowSettings(pSettings, mHelpWindow);
}

//==============================================================================

void HelpPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our help window settings

    saveWindowSettings(pSettings, mHelpWindow);
}

//==============================================================================

void HelpPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void HelpPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * HelpPlugin::viewWidget(const QString &pFileName,
                                 const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void HelpPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString HelpPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon HelpPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool HelpPlugin::saveFile(const QString &pOldFileName,
                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void HelpPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...
}

//==============================================================================

void HelpPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool HelpPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void HelpPlugin::retranslateUi()
{
    // Retranslate our help action

    retranslateAction(mHelpAction, tr("Help"),
                      tr("Show/hide the OpenCOR help"));

    // Retranslate our help window

    mHelpWindow->retranslateUi();
}

//==============================================================================

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
