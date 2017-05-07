/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Sample window plugin
//==============================================================================

#include "coreguiutils.h"
#include "samplewindowplugin.h"
#include "samplewindowwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace SampleWindow {

//==============================================================================

PLUGININFO_FUNC SampleWindowPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides an addition window."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit une fenêtre d'addition."));

    return new PluginInfo(PluginInfo::Sample, true, false,
                          QStringList() << "Core" << "Sample",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void SampleWindowPlugin::retranslateUi()
{
    // Retranslate our Sample window action

    retranslateAction(mSampleWindowAction,
                      tr("Sample"),
                      tr("Show/hide the Sample window"));
}

//==============================================================================
// Plugin interface
//==============================================================================

bool SampleWindowPlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool SampleWindowPlugin::pluginInterfacesOk(const QString &pFileName,
                                            QObject *pInstance)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pInstance);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void SampleWindowPlugin::initializePlugin()
{
    // Create an action to show/hide our Sample window

    mSampleWindowAction = Core::newAction(true, Core::mainWindow());

    // Create our Sample window

    mSampleWindowWindow = new SampleWindowWindow(Core::mainWindow());
}

//==============================================================================

void SampleWindowPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SampleWindowPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void SampleWindowPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleWindowPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void SampleWindowPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Window interface
//==============================================================================

Qt::DockWidgetArea SampleWindowPlugin::windowDefaultDockArea() const
{
    // Return our default dock area

    return Qt::TopDockWidgetArea;
}

//==============================================================================

QAction * SampleWindowPlugin::windowAction() const
{
    // Return our window action

    return mSampleWindowAction;
}

//==============================================================================

QDockWidget * SampleWindowPlugin::windowWidget() const
{
    // Return our window widget

    return mSampleWindowWindow;
}

//==============================================================================

}   // namespace SampleWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
