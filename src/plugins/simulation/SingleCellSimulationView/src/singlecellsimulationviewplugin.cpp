//==============================================================================
// SingleCellSimulationView plugin
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "singlecellsimulationviewplugin.h"
#include "singlecellsimulationviewwidget.h"
#include "solverinterface.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

PLUGININFO_FUNC SingleCellSimulationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to run single cell simulations");
    descriptions.insert("fr", "Une extension pour exécuter des simulations unicellulaires");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSimulation" << "CellMLSupport" << "Qwt",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(SingleCellSimulationView, SingleCellSimulationViewPlugin)

//==============================================================================

SingleCellSimulationViewPlugin::SingleCellSimulationViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Simulation,
                          QStringList() << CellMLSupport::CellmlMimeType);
}

//==============================================================================

void SingleCellSimulationViewPlugin::initialize()
{
    // Create our single view widget

    mViewWidget = new SingleCellSimulationViewWidget(mMainWindow);

    // Hide our single view widget since it may not initially be shown in our
    // central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void SingleCellSimulationViewPlugin::initializationsDone(const Plugins &pLoadedPlugins)
{
    // Retrieve the different solvers that are available to us

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(loadedPlugin->instance());

        if (solverInterface)
            // The plugin implements our solver interface, so...

            mViewWidget->addSolverInterface(solverInterface);
    }
}

//==============================================================================

void SingleCellSimulationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our single cell simulation view settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void SingleCellSimulationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our single cell simulation view settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

QWidget * SingleCellSimulationViewPlugin::viewWidget(const QString &pFileName)
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

QString SingleCellSimulationViewPlugin::viewName()
{
    // Return our single cell view's name

    return tr("Single Cell");
}

//==============================================================================

void SingleCellSimulationViewPlugin::retranslateUi()
{
    // Retranslate our single cell simulation view

    mViewWidget->retranslateUi();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
