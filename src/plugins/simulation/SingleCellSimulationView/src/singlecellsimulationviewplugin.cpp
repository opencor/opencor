//==============================================================================
// SingleCellSimulationView plugin
//==============================================================================

#include "cellmlfilemanager.h"
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

    return PluginInfo(PluginInfo::V001,
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

    mGuiSettings->addView(GuiViewSettings::Simulation, 0);
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

QWidget * SingleCellSimulationViewPlugin::viewWidget(const QString &pFileName,
                                                     const int &)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic simulation view widget

    CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);

    if (!cellmlFile)
        // We are not dealing with a CellML file, so...

        return GuiInterface::newViewWidget(pFileName);

    // We are dealing with a CellML file, so update our generic simulation view
    // widget using the passed CellML file

    mViewWidget->initialize(pFileName);

    // Our generic simulation view widget is now ready, so...

    return mViewWidget;
}

//==============================================================================

QString SingleCellSimulationViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("Single Cell");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
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
