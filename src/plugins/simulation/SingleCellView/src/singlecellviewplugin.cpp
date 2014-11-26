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
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

PLUGININFO_FUNC SingleCellViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to run single cell simulations."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour exécuter des simulations unicellulaires."));

    return new PluginInfo(PluginInfo::Simulation, true, false,
                          QStringList() << "CellMLSupport" << "CSVDataStore" << "Qwt",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool SingleCellViewPlugin::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // Make sure that we are dealing with a CellML file
    // Note: we can't modify a CellML file using this view, hence the below is
    //       only for the case where the user wants to save a CellML under a new
    //       name...

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

void SingleCellViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so let its corresponding view widget
    // know about it

    if (mViewWidget->contains(pFileName))
        mViewWidget->fileReloaded(pFileName);
}

//==============================================================================

void SingleCellViewPlugin::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // Let our view widget know that a file has been renamed

    mViewWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void SingleCellViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::fileClosed(const QString &pFileName)
{
    // Let our view widget know that a file has been closed

    mViewWidget->fileClosed(pFileName);
}

//==============================================================================
// I18n interface
//==============================================================================

void SingleCellViewPlugin::retranslateUi()
{
    // Retranslate our single cell view widget

    mViewWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

void SingleCellViewPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create our single view widget

    mViewWidget = new SingleCellViewWidget(this, pMainWindow);

    // Hide our single view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void SingleCellViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void SingleCellViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Retrieve the different solvers that are available to us

    SolverInterfaces solverInterfaces = SolverInterfaces();

    foreach (Plugin *plugin, pLoadedPlugins) {
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(plugin->instance());

        if (solverInterface)
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

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our single cell view settings

    pSettings->beginGroup(mViewWidget->objectName());
        mViewWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// View interface
//==============================================================================

ViewInterface::Mode SingleCellViewPlugin::viewMode() const
{
    // Return our mode

    return ViewInterface::Simulation;
}

//==============================================================================

QStringList SingleCellViewPlugin::viewMimeTypes() const
{
    // Return the MIME types we support

    return QStringList() << CellMLSupport::CellmlMimeType;
}

//==============================================================================

QWidget * SingleCellViewPlugin::viewWidget(const QString &pFileName)
{
    // Make sure that we are dealing with a CellML file

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        return 0;

    // Update and return our simulation view widget using the given CellML file

    mViewWidget->initialize(pFileName);

    return mViewWidget;
}

//==============================================================================

void SingleCellViewPlugin::removeViewWidget(const QString &pFileName)
{
    // Ask our view widget to finalise the given CellML file

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

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
