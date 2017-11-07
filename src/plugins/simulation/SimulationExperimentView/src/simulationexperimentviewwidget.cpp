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
// Simulation Experiment view widget
//==============================================================================

#include "collapsiblewidget.h"
#include "filemanager.h"
#include "simulation.h"
#include "simulationexperimentviewcontentswidget.h"
#include "simulationexperimentviewinformationgraphpanelandgraphswidget.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewinformationsolverswidget.h"
#include "simulationexperimentviewinformationwidget.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QLayout>
#include <QSettings>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewWidget::SimulationExperimentViewWidget(SimulationExperimentViewPlugin *pPlugin,
                                                               const Plugins &pCellmlEditingViewPlugins,
                                                               const Plugins &pCellmlSimulationViewPlugins,
                                                               QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mCellmlEditingViewPlugins(pCellmlEditingViewPlugins),
    mCellmlSimulationViewPlugins(pCellmlSimulationViewPlugins),
    mSimulationWidgetSizes(QIntList()),
    mContentsWidgetSizes(QIntList()),
    mCollapsibleWidgetCollapsed(QBoolList()),
    mGraphPanelGraphsMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Graphs),
    mSimulationColumnWidths(QIntList()),
    mSolversColumnWidths(QIntList()),
    mGraphPanelColumnWidths(QIntList()),
    mGraphsColumnWidths(QIntList()),
    mParametersColumnWidths(QIntList()),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SimulationExperimentViewSimulationWidget *>()),
    mFileNames(QStringList()),
    mSimulationResultsSizes(QMap<QString, qulonglong>()),
    mSimulationCheckResults(QStringList())
{
}

//==============================================================================

static const auto SettingsSizes                  = QStringLiteral("Sizes");
static const auto SettingsContentsSizes          = QStringLiteral("ContentsSizes");
static const auto SettingsCollapsed              = QStringLiteral("Collapsed");
static const auto SettingsGraphPanelGraphsMode   = QStringLiteral("GraphPanelGraphsMode");
static const auto SettingsSimulationColumnWidths = QStringLiteral("SimulationColumnWidths");
static const auto SettingsSolversColumnWidths    = QStringLiteral("SolversColumnWidths");
static const auto SettingsGraphPanelColumnWidths = QStringLiteral("GraphPanelColumnWidths");
static const auto SettingsGraphsColumnWidths     = QStringLiteral("GraphsColumnWidths");
static const auto SettingsParametersColumnWidths = QStringLiteral("ParametersColumnWidths");

//==============================================================================

void SimulationExperimentViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the sizes of our simulation widget and of its contents widget

    static const QVariantList defaultContentsSizes = QVariantList() << 0.25*qApp->desktop()->screenGeometry().width()
                                                                       << 0.75*qApp->desktop()->screenGeometry().width();

    mSimulationWidgetSizes = qVariantListToIntList(pSettings->value(SettingsSizes).toList());
    mContentsWidgetSizes = qVariantListToIntList(pSettings->value(SettingsContentsSizes, defaultContentsSizes).toList());

    // Retrieve the collapsed states of our collapsible widget

    static const QVariantList defaultCollapsed = QVariantList() << false << false << false;

    mCollapsibleWidgetCollapsed = qVariantListToBoolList(pSettings->value(SettingsCollapsed, defaultCollapsed).toList());

    // Retrieve our graph panel /graphs mode

    mGraphPanelGraphsMode = SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode(pSettings->value(SettingsGraphPanelGraphsMode, SimulationExperimentViewInformationGraphPanelAndGraphsWidget::GraphPanel).toInt());

    // Retrieve the columns' width of our various property editors

    static const QVariantList defaultColumnWidths = QVariantList() << 100 << 100 << 100;

    mSimulationColumnWidths = qVariantListToIntList(pSettings->value(SettingsSimulationColumnWidths, defaultColumnWidths).toList());
    mSolversColumnWidths = qVariantListToIntList(pSettings->value(SettingsSolversColumnWidths, defaultColumnWidths).toList());
    mGraphPanelColumnWidths = qVariantListToIntList(pSettings->value(SettingsGraphPanelColumnWidths, defaultColumnWidths).toList());
    mGraphsColumnWidths = qVariantListToIntList(pSettings->value(SettingsGraphsColumnWidths, defaultColumnWidths).toList());
    mParametersColumnWidths = qVariantListToIntList(pSettings->value(SettingsParametersColumnWidths, defaultColumnWidths).toList());
}

//==============================================================================

void SimulationExperimentViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the sizes of our simulation widget and those of its
    // contents widget

    pSettings->setValue(SettingsSizes, qIntListToVariantList(mSimulationWidgetSizes));
    pSettings->setValue(SettingsContentsSizes, qIntListToVariantList(mContentsWidgetSizes));

    // Keep track of the collapsed states of our collapsible widget

    pSettings->setValue(SettingsCollapsed, qBoolListToVariantList(mCollapsibleWidgetCollapsed));

    // Keep track of our graph panel /graphs mode

    pSettings->setValue(SettingsGraphPanelGraphsMode, mGraphPanelGraphsMode);

    // Keep track of the columns' width of our various property editors

    pSettings->setValue(SettingsSimulationColumnWidths, qIntListToVariantList(mSimulationColumnWidths));
    pSettings->setValue(SettingsSolversColumnWidths, qIntListToVariantList(mSolversColumnWidths));
    pSettings->setValue(SettingsGraphPanelColumnWidths, qIntListToVariantList(mGraphPanelColumnWidths));
    pSettings->setValue(SettingsGraphsColumnWidths, qIntListToVariantList(mGraphsColumnWidths));
    pSettings->setValue(SettingsParametersColumnWidths, qIntListToVariantList(mParametersColumnWidths));
}

//==============================================================================

void SimulationExperimentViewWidget::retranslateUi()
{
    // Retranslate our simulation widgets

    foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets)
        simulationWidget->retranslateUi();
}

//==============================================================================

void SimulationExperimentViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the simulation widget associated with the given file, if any

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (!mSimulationWidget) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SimulationExperimentViewSimulationWidget(mPlugin, this, pFileName, this);

        // Keep track of our editing widget

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        // Initialise our simulation widget

        mSimulationWidget->initialize();

        // Keep track of various things related to our simulation widget and its
        // children

        connect(mSimulationWidget, SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(simulationWidgetSplitterMoved(const QIntList &)));

        SimulationExperimentViewContentsWidget *contentsWidget = mSimulationWidget->contentsWidget();

        connect(contentsWidget, SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(contentsWidgetSplitterMoved(const QIntList &)));

        SimulationExperimentViewInformationWidget *informationWidget = contentsWidget->informationWidget();

        connect(informationWidget->collapsibleWidget(), SIGNAL(collapsed(const int &, const bool &)),
                this, SLOT(collapsibleWidgetCollapsed(const int &, const bool &)));

        connect(informationWidget->graphPanelAndGraphsWidget(), SIGNAL(graphPanelGraphsModeChanged(const OpenCOR::SimulationExperimentView::SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode &)),
                this, SLOT(graphPanelGraphsModeChanged(const OpenCOR::SimulationExperimentView::SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode &)));

        connect(informationWidget->simulationWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(simulationHeaderSectionResized(const int &, const int &, const int &)));
        connect(informationWidget->solversWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(solversHeaderSectionResized(const int &, const int &, const int &)));
        connect(informationWidget->graphPanelAndGraphsWidget(), SIGNAL(graphPanelHeaderSectionResized(int, int, int)),
                this, SLOT(graphPanelHeaderSectionResized(const int &, const int &, const int &)));
        connect(informationWidget->graphPanelAndGraphsWidget(), SIGNAL(graphsHeaderSectionResized(int, int, int)),
                this, SLOT(graphsHeaderSectionResized(const int &, const int &, const int &)));
        connect(informationWidget->parametersWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(parametersHeaderSectionResized(const int &, const int &, const int &)));

        // Check when some graph plot settings or graphs settings have been
        // requested

        connect(mSimulationWidget, SIGNAL(graphPanelSettingsRequested()),
                this, SLOT(graphPanelSettingsRequested()));
        connect(mSimulationWidget, SIGNAL(graphsSettingsRequested()),
                this, SLOT(graphsSettingsRequested()));
    } else {
        // We already have a simulation widget, so just make sure that its GUI
        // is up to date

        mSimulationWidget->updateGui();
    }

    // Update our new simualtion widget and its children, if needed

    mSimulationWidget->setSizes(mSimulationWidgetSizes);
    mSimulationWidget->contentsWidget()->setSizes(mContentsWidgetSizes);

    // Update some of our simulation's contents' information GUI

    updateContentsInformationGui(mSimulationWidget);

    // Set our focus proxy to our 'new' simulation widget and make sure that the
    // latter immediately gets the focus

    setFocusProxy(mSimulationWidget);

    mSimulationWidget->setFocus();
}

//==============================================================================

void SimulationExperimentViewWidget::finalize(const QString &pFileName)
{
    // Remove the simulation widget, should there be one for the given file

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        // Finalise our simulation widget

        simulationWidget->finalize();

        // Now, we can delete it and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Next, we reset our memory of the simulation widget, if needed

        if (simulationWidget == mSimulationWidget)
            mSimulationWidget = 0;
    }
}

//==============================================================================

QIcon SimulationExperimentViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return, if possible, the tab icon for the given file

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        return simulationWidget->fileTabIcon();
    } else {
        static const QIcon NoIcon = QIcon();

        return NoIcon;
    }
}

//==============================================================================

bool SimulationExperimentViewWidget::saveFile(const QString &pOldFileName,
                                              const QString &pNewFileName)
{
    // Save the given file, if possible

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget)
        return simulationWidget->save(pNewFileName);
    else
        return false;
}

//==============================================================================

void SimulationExperimentViewWidget::fileOpened(const QString &pFileName)
{
    // Track the given file name

    mFileNames << pFileName;

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
}

//==============================================================================

void SimulationExperimentViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been un/locked

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->filePermissionsChanged();
}

//==============================================================================

void SimulationExperimentViewWidget::fileModified(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been modified

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileModified();
}

//==============================================================================

void SimulationExperimentViewWidget::fileReloaded(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been reloaded

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        simulationWidget->fileReloaded();

        // Make sure that our simulation's contents' information GUI is up to
        // date
        // Note: this is, at least, necessary for our paramaters widget since we
        //       repopulate it, meaning that its columns' width will be reset...

        updateContentsInformationGui(simulationWidget);

        // Make sure that the GUI of our simulation widgets is up to date

        foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
            simulationWidget->updateGui(true);
    }
}

//==============================================================================

void SimulationExperimentViewWidget::fileRenamed(const QString &pOldFileName,
                                                 const QString &pNewFileName)
{
    // Stop tracking the old given file name and track the given new one instead

    mFileNames.removeOne(pOldFileName);

    mFileNames << pNewFileName;

    // The given file has been renamed, so update our simulation widgets mapping

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget) {
        simulationWidget->setFileName(pNewFileName);

        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }

    // Let our simulation widgets know that a file has been renamed

    foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void SimulationExperimentViewWidget::fileClosed(const QString &pFileName)
{
    // Stop tracking the given file name

    mFileNames.removeOne(pFileName);

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
}

//==============================================================================

QStringList SimulationExperimentViewWidget::fileNames() const
{
    // Return our file names

    return mFileNames;
}

//==============================================================================

Plugins SimulationExperimentViewWidget::cellmlEditingViewPlugins() const
{
    // Return our CellML editing view plugins

    return mCellmlEditingViewPlugins;
}

//==============================================================================

Plugins SimulationExperimentViewWidget::cellmlSimulationViewPlugins() const
{
    // Return our CellML simulation view plugins

    return mCellmlSimulationViewPlugins;
}

//==============================================================================

SimulationExperimentViewSimulationWidget * SimulationExperimentViewWidget::simulationWidget(const QString &pFileName) const
{
    // Return the requested simulation widget

    return mSimulationWidgets.value(pFileName);
}

//==============================================================================

SimulationSupport::Simulation * SimulationExperimentViewWidget::simulation(const QString &pFileName) const
{
    // Return the simulation for the given file name

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        return simulationWidget->simulation();
    else
        return 0;
}

//==============================================================================

CellMLSupport::CellmlFileRuntime * SimulationExperimentViewWidget::runtime(const QString &pFileName) const
{
    // Return the runtime for the given file name

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        return simulationWidget->simulation()->runtime();
    else
        return 0;
}

//==============================================================================

QWidget * SimulationExperimentViewWidget::widget(const QString &pFileName)
{
    // Return the requested (simulation) widget

    return simulationWidget(pFileName);
}

//==============================================================================

qulonglong SimulationExperimentViewWidget::simulationResultsSize(const QString &pFileName) const
{
    // Return the results size for the given file name

    return mSimulationResultsSizes.value(pFileName);
}

//==============================================================================

void SimulationExperimentViewWidget::checkSimulationResults(const QString &pFileName,
                                                            const bool &pClearGraphs)
{
    // Make sure that we can still check results (i.e. we are not closing down
    // with some simulations still running)

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (!simulationWidget)
        return;

    // Update all of our simulation widgets' results, but only if needed
    // Note: to update only the given simulation widget's results is not enough
    //       since another simulation widget may have graphs that refer to the
    //       given simulation widget...

    SimulationSupport::Simulation *simulation = simulationWidget->simulation();
    qulonglong simulationResultsSize = simulation->results()->size();

    if (   pClearGraphs
        || (simulationResultsSize != mSimulationResultsSizes.value(pFileName))) {
        mSimulationResultsSizes.insert(pFileName, simulationResultsSize);

        foreach (SimulationExperimentViewSimulationWidget *currentSimulationWidget, mSimulationWidgets)
            currentSimulationWidget->updateSimulationResults(simulationWidget, simulationResultsSize, pClearGraphs);
    }

    // Ask to recheck our simulation widget's results, but only if its
    // simulation is still running

    if (   simulation->isRunning()
        || (simulationResultsSize != simulation->results()->size())) {
        // Note: we cannot ask QTimer::singleShot() to call
        //       callCheckSimulationResults() since it expects a simulation
        //       widget as a parameter, so instead we call a method with no
        //       arguments that will make use of our list to know which
        //       simulation should be passed as an argument to
        //       checkSimulationResults()...

        mSimulationCheckResults << pFileName;

        QTimer::singleShot(0, this, SLOT(callCheckSimulationResults()));
    } else if (!simulation->isRunning() && !simulation->isPaused()) {
        // The simulation is over, so stop tracking the result's size and reset
        // the simulation progress of the given file

        mSimulationResultsSizes.remove(pFileName);

        simulationWidget->resetSimulationProgress();
    }
}

//==============================================================================

void SimulationExperimentViewWidget::callCheckSimulationResults()
{
    // Retrieve the simulation widget for which we want to call checkResults()
    // and then call checkResults() for it

    QString fileName = mSimulationCheckResults.first();

    mSimulationCheckResults.removeFirst();

    checkSimulationResults(fileName);
}

//==============================================================================

void SimulationExperimentViewWidget::simulationWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our simulation widget has moved, so keep track of its new
    // sizes

    mSimulationWidgetSizes = pSizes;
}

//==============================================================================

void SimulationExperimentViewWidget::contentsWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our contents widget has moved, so keep track of its new
    // sizes

    mContentsWidgetSizes = pSizes;
}

//==============================================================================

void SimulationExperimentViewWidget::collapsibleWidgetCollapsed(const int &pIndex,
                                                                const bool &pCollapsed)
{
    // One of the widgets in our collapsible widget has been collapsed or
    // expanded, so keep track of that fact

    mCollapsibleWidgetCollapsed[pIndex] = pCollapsed;
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelSettingsRequested()
{
    // Make sure that our graph panel settings are active and visible

    mSimulationWidget->contentsWidget()->informationWidget()->graphPanelAndGraphsWidget()->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::GraphPanel);
    mSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget()->setCollapsed(2, false);
}

//==============================================================================

void SimulationExperimentViewWidget::graphsSettingsRequested()
{
    // Make sure that our graphs settings are active and visible

    mSimulationWidget->contentsWidget()->informationWidget()->graphPanelAndGraphsWidget()->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Graphs);
    mSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget()->setCollapsed(2, false);
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelGraphsModeChanged(const OpenCOR::SimulationExperimentView::SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode &pMode)
{
    // Keep track of the new graph panel / graphs mode

    mGraphPanelGraphsMode = pMode;
}

//==============================================================================

void SimulationExperimentViewWidget::simulationHeaderSectionResized(const int &pIndex,
                                                                    const int &pOldSize,
                                                                    const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mSimulationColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::solversHeaderSectionResized(const int &pIndex,
                                                                 const int &pOldSize,
                                                                 const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mSolversColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelHeaderSectionResized(const int &pIndex,
                                                                    const int &pOldSize,
                                                                    const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mGraphPanelColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphsHeaderSectionResized(const int &pIndex,
                                                                const int &pOldSize,
                                                                const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mGraphsColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::parametersHeaderSectionResized(const int &pIndex,
                                                                    const int &pOldSize,
                                                                    const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mParametersColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::updateContentsInformationGui(SimulationExperimentViewSimulationWidget *pSimulationWidget)
{
    // Update some of our simulation's contents' information GUI
    // Note: for column widths, we set the last column width to zero to avoid
    //       potential issues, should the vertical scrollbar be visible...

    SimulationExperimentViewInformationWidget *informationWidget = pSimulationWidget->contentsWidget()->informationWidget();

    for (int i = 0, iMax = mCollapsibleWidgetCollapsed.count(); i < iMax; ++i)
        informationWidget->collapsibleWidget()->setCollapsed(i, mCollapsibleWidgetCollapsed[i]);

    informationWidget->graphPanelAndGraphsWidget()->setMode(mGraphPanelGraphsMode);

    for (int i = 0, iMax = mSimulationColumnWidths.count(); i < iMax; ++i)
        informationWidget->simulationWidget()->setColumnWidth(i, (i == iMax-1)?0:mSimulationColumnWidths[i]);

    for (int i = 0, iMax = mSolversColumnWidths.count(); i < iMax; ++i)
        informationWidget->solversWidget()->setColumnWidth(i, (i == iMax-1)?0:mSolversColumnWidths[i]);

    for (int i = 0, iMax = mGraphPanelColumnWidths.count(); i < iMax; ++i)
        informationWidget->graphPanelAndGraphsWidget()->setGraphPanelColumnWidth(i, (i == iMax-1)?0:mGraphPanelColumnWidths[i]);

    for (int i = 0, iMax = mGraphsColumnWidths.count(); i < iMax; ++i)
        informationWidget->graphPanelAndGraphsWidget()->setGraphsColumnWidth(i, (i == iMax-1)?0:mGraphsColumnWidths[i]);

    for (int i = 0, iMax = mParametersColumnWidths.count(); i < iMax; ++i)
        informationWidget->parametersWidget()->setColumnWidth(i, (i == iMax-1)?0:mParametersColumnWidths[i]);
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
