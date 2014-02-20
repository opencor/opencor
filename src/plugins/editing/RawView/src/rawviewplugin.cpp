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
// RawView plugin
//==============================================================================

#include "cliutils.h"
#include "rawviewplugin.h"
#include "rawviewwidget.h"

//==============================================================================

#include <QMainWindow>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

PLUGININFO_FUNC RawViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to edit files using a text editor."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour éditer des fichiers à l'aide d'un éditeur de texte."));

    return new PluginInfo(PluginInfo::Editing, true,
                          QStringList() << "CoreEditing",
                          descriptions);
}

//==============================================================================

RawViewPlugin::RawViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing, QStringList());
}

//==============================================================================
// Core interface
//==============================================================================

void RawViewPlugin::initialize()
{
    // Create our generic raw view widget

    mViewWidget = new RawViewWidget(mMainWindow);

    // Hide our generic raw view widget since it may not initially be shown in
    // our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our generic raw view widget settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void RawViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our generic raw view widget settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void RawViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Editing interface
//==============================================================================

QScintillaSupport::QScintillaWidget * RawViewPlugin::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mViewWidget->editor(pFileName);
}

//==============================================================================
// GUI interface
//==============================================================================

void RawViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool RawViewPlugin::hasViewWidget(const QString &pFileName)
{
    // Return whether we know about the given file

    return mViewWidget->contains(pFileName);;
}

//==============================================================================

QWidget * RawViewPlugin::viewWidget(const QString &pFileName,
                                    const bool &pCreate)
{
    // Update our generic raw view widget using the given file

    if (pCreate) {
        mViewWidget->initialize(pFileName);

        return mViewWidget;
    } else {
        return 0;
    }
}

//==============================================================================

void RawViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our generic raw view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString RawViewPlugin::viewName() const
{
    // Return our raw view's name

    return tr("Raw");
}

//==============================================================================

QIcon RawViewPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool RawViewPlugin::saveFile(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    // Ask our generic raw view widget to save the given file

    QScintillaSupport::QScintillaWidget *editor = mViewWidget->editor(pOldFileName);
    bool res = Core::writeTextToFile(pNewFileName, editor->contents());

    if (res)
        editor->resetUndoHistory();

    return res;
}

//==============================================================================

void RawViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileModified(const QString &pFileName,
                                 const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

    // We don't handle this interface...
}

//==============================================================================

void RawViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void RawViewPlugin::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // The given file has been renamed, so let our view widget know about it

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void RawViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

bool RawViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void RawViewPlugin::retranslateUi()
{
    // We don't handle this interface...
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
