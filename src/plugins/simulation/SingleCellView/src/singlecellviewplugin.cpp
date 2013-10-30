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
// SingleCellView plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "singlecellviewplugin.h"
#include "singlecellviewwidget.h"
#include "solverinterface.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

PLUGININFO_FUNC SingleCellViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to run single cell simulations."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour exécuter des simulations unicellulaires."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Simulation,
                          false,
                          true,
                          QStringList() << "CellMLSupport" << "Qwt",
                          descriptions);
}

//==============================================================================

SingleCellViewPlugin::SingleCellViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Simulation,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================
// Core interface
//==============================================================================

void SingleCellViewPlugin::initialize()
{
    // Create our single view widget

    mViewWidget = new SingleCellViewWidget(this, mMainWindow);

    // Hide our single view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void SingleCellViewPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::initialized(const Plugins &pLoadedPlugins)
{
    // Retrieve the different solvers that are available to us

    SolverInterfaces solverInterfaces = SolverInterfaces();

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(loadedPlugin->instance());

        if (solverInterface)
            // The plugin implements our solver interface, so...

            solverInterfaces << solverInterface;
    }

    // Initialise our view widget with the different solvers that are available
    // to us

    mViewWidget->setSolverInterfaces(solverInterfaces);
}

//==============================================================================

void SingleCellViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our single cell view settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void SingleCellViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our single cell view settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void SingleCellViewPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::runCliCommand(const QString &pCommand,
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

void SingleCellViewPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::updateGui(Plugin *pViewPlugin,
                                     const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

QWidget * SingleCellViewPlugin::viewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic simulation view widget after having initialised it

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // We are dealing with a CellML file, so update our generic simulation view
    // widget using the given CellML file

    mViewWidget->initialize(pFileName);

    // Our generic simulation view widget is now ready, so...

    return mViewWidget;
}

//==============================================================================

void SingleCellViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our generic view widget to finalise the given file

    mViewWidget->finalize(pFileName);
}

//==============================================================================

QString SingleCellViewPlugin::viewName() const
{
    // Return our single cell view's name

    return tr("Single Cell");
}

//==============================================================================

QIcon SingleCellViewPlugin::fileTabIcon(const QString &pFileName) const
{
    // Return the requested file tab icon

    return mViewWidget->fileTabIcon(pFileName);
}

//==============================================================================

bool SingleCellViewPlugin::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // Make sure that we are dealing with a CellML file

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pOldFileName);

    return cellmlFile?cellmlFile->save(pNewFileName):false;
}

//==============================================================================

void SingleCellViewPlugin::fileOpened(const QString &pFileName)
{
    // Let our view widget know that a file has been opened

    mViewWidget->fileOpened(pFileName);
}

//==============================================================================

void SingleCellViewPlugin::fileReloaded(const QString &pFileName)
{
    // The file has been reloaded, so let our view widget know about it

    mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void SingleCellViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // Let our view widget know that a file has been renamed

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void SingleCellViewPlugin::fileClosed(const QString &pFileName)
{
    // Let our view widget know that a file has been closed

    mViewWidget->fileClosed(pFileName);
}

//==============================================================================

bool SingleCellViewPlugin::canClose()
{
    // We don't handle this interface...

    return true;
}

//==============================================================================
// I18n interface
//==============================================================================

void SingleCellViewPlugin::retranslateUi()
{
    // Retranslate our single cell view

    mViewWidget->retranslateUi();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
