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
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QHeaderView>
#include <QLayout>
#include <QScreen>
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
    mCellmlSimulationViewPlugins(pCellmlSimulationViewPlugins)
{
}

//==============================================================================

static const char *SettingsSizes                  = "Sizes";
static const char *SettingsContentsSizes          = "ContentsSizes";
static const char *SettingsCollapsed              = "Collapsed";
static const char *SettingsGraphPanelGraphsMode   = "GraphPanelGraphsMode";
static const char *SettingsSimulationColumnWidths = "SimulationColumnWidths";
static const char *SettingsSolversColumnWidths    = "SolversColumnWidths";
static const char *SettingsGraphPanelColumnWidths = "GraphPanelColumnWidths";
static const char *SettingsGraphsColumnWidths     = "GraphsColumnWidths";
static const char *SettingsParametersColumnWidths = "ParametersColumnWidths";

//==============================================================================

void SimulationExperimentViewWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve the sizes of our simulation widget and of its contents widget

    int availableGeometryWidth = qApp->primaryScreen()->availableGeometry().width();
    QVariantList defaultContentsSizes = QVariantList() << 0.25*availableGeometryWidth
                                                       << 0.75*availableGeometryWidth;

    mSimulationWidgetSizes = qVariantListToIntList(pSettings.value(SettingsSizes).toList());
    mContentsWidgetSizes = qVariantListToIntList(pSettings.value(SettingsContentsSizes, defaultContentsSizes).toList());

    // Retrieve the collapsed states of our collapsible widget

    static const QVariantList defaultCollapsed = QVariantList() << false << false << false;

    mCollapsibleWidgetCollapsed = qVariantListToBoolList(pSettings.value(SettingsCollapsed, defaultCollapsed).toList());

    // Retrieve our graph panel /graphs mode

    mGraphPanelGraphsMode = SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode(pSettings.value(SettingsGraphPanelGraphsMode, int(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::Graphs)).toInt());

    // Retrieve the columns' width of our various property editors

    static const QVariantList defaultThreeColumnWidths = QVariantList() << 100 << 100 << 100;
    static const QVariantList defaultTwoColumnWidths = QVariantList() << 100 << 100;

    mSimulationColumnWidths = qVariantListToIntList(pSettings.value(SettingsSimulationColumnWidths, defaultThreeColumnWidths).toList());
    mSolversColumnWidths = qVariantListToIntList(pSettings.value(SettingsSolversColumnWidths, defaultThreeColumnWidths).toList());
    mGraphPanelColumnWidths = qVariantListToIntList(pSettings.value(SettingsGraphPanelColumnWidths, defaultTwoColumnWidths).toList());
    mGraphsColumnWidths = qVariantListToIntList(pSettings.value(SettingsGraphsColumnWidths, defaultTwoColumnWidths).toList());
    mParametersColumnWidths = qVariantListToIntList(pSettings.value(SettingsParametersColumnWidths, defaultThreeColumnWidths).toList());
}

//==============================================================================

void SimulationExperimentViewWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the sizes of our simulation widget and those of its
    // contents widget

    pSettings.setValue(SettingsSizes, qIntListToVariantList(mSimulationWidgetSizes));
    pSettings.setValue(SettingsContentsSizes, qIntListToVariantList(mContentsWidgetSizes));

    // Keep track of the collapsed states of our collapsible widget

    pSettings.setValue(SettingsCollapsed, qBoolListToVariantList(mCollapsibleWidgetCollapsed));

    // Keep track of our graph panel /graphs mode

    pSettings.setValue(SettingsGraphPanelGraphsMode, int(mGraphPanelGraphsMode));

    // Keep track of the columns' width of our various property editors

    pSettings.setValue(SettingsSimulationColumnWidths, qIntListToVariantList(mSimulationColumnWidths));
    pSettings.setValue(SettingsSolversColumnWidths, qIntListToVariantList(mSolversColumnWidths));
    pSettings.setValue(SettingsGraphPanelColumnWidths, qIntListToVariantList(mGraphPanelColumnWidths));
    pSettings.setValue(SettingsGraphsColumnWidths, qIntListToVariantList(mGraphsColumnWidths));
    pSettings.setValue(SettingsParametersColumnWidths, qIntListToVariantList(mParametersColumnWidths));
}

//==============================================================================

void SimulationExperimentViewWidget::retranslateUi()
{
    // Retranslate our simulation widgets

    for (auto simulationWidget : mSimulationWidgets) {
        simulationWidget->retranslateUi();
    }
}

//==============================================================================

void SimulationExperimentViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the simulation widget associated with the given file, if any

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (mSimulationWidget == nullptr) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SimulationExperimentViewSimulationWidget(mPlugin, this, pFileName, this);

        // Keep track of our editing widget

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        // Initialise our simulation widget

        mSimulationWidget->initialize();

        // Keep track of various things related to our simulation widget and its
        // children

        connect(mSimulationWidget, &SimulationExperimentViewSimulationWidget::splitterMoved,
                this, &SimulationExperimentViewWidget::simulationWidgetSplitterMoved);

        SimulationExperimentViewContentsWidget *contentsWidget = mSimulationWidget->contentsWidget();

        connect(contentsWidget, &SimulationExperimentViewContentsWidget::splitterMoved,
                this, &SimulationExperimentViewWidget::contentsWidgetSplitterMoved);

        SimulationExperimentViewInformationWidget *informationWidget = contentsWidget->informationWidget();

        connect(informationWidget->collapsibleWidget(), &Core::CollapsibleWidget::collapsed,
                this, &SimulationExperimentViewWidget::collapsibleWidgetCollapsed);

        connect(informationWidget->graphPanelAndGraphsWidget(), &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelGraphsModeChanged,
                this, &SimulationExperimentViewWidget::graphPanelGraphsModeChanged);

        connect(informationWidget->simulationWidget()->header(), &QHeaderView::sectionResized,
                this, &SimulationExperimentViewWidget::simulationHeaderSectionResized);
        connect(informationWidget->solversWidget()->header(), &QHeaderView::sectionResized,
                this, &SimulationExperimentViewWidget::solversHeaderSectionResized);
        connect(informationWidget->graphPanelAndGraphsWidget(), &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelHeaderSectionResized,
                this, &SimulationExperimentViewWidget::graphPanelHeaderSectionResized);
        connect(informationWidget->graphPanelAndGraphsWidget(), &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphsHeaderSectionResized,
                this, &SimulationExperimentViewWidget::graphsHeaderSectionResized);
        connect(informationWidget->parametersWidget()->header(), &QHeaderView::sectionResized,
                this, &SimulationExperimentViewWidget::parametersHeaderSectionResized);

        connect(informationWidget->graphPanelAndGraphsWidget(), QOverload<int, bool>::of(&SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelSectionExpanded),
                this, &SimulationExperimentViewWidget::graphPanelSectionExpanded);

        // Check when some graph plot settings or graphs settings have been
        // requested

        connect(mSimulationWidget, &SimulationExperimentViewSimulationWidget::graphPanelSettingsRequested,
                this, &SimulationExperimentViewWidget::graphPanelSettingsRequested);
        connect(mSimulationWidget, &SimulationExperimentViewSimulationWidget::graphsSettingsRequested,
                this, &SimulationExperimentViewWidget::graphsSettingsRequested);
    } else {
        // We already have a simulation widget, so just make sure that its GUI
        // is up to date

        mSimulationWidget->updateGui();
    }

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

    if (simulationWidget != nullptr) {
        // Finalise our simulation widget

        simulationWidget->finalize();

        // Now, we can delete it and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Next, we reset our memory of the simulation widget, if needed

        if (simulationWidget == mSimulationWidget) {
            mSimulationWidget = nullptr;
        }
    }
}

//==============================================================================

QIcon SimulationExperimentViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return, if possible, the tab icon for the given file

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget != nullptr) {
        return simulationWidget->fileTabIcon();
    }

    static const QIcon NoIcon = QIcon();

    return NoIcon;
}

//==============================================================================

bool SimulationExperimentViewWidget::importFile(const QString &pFileName)
{
    // Import the given file into the current simulation widget

    return mSimulationWidget->import(pFileName, false);
}

//==============================================================================

bool SimulationExperimentViewWidget::saveFile(const QString &pOldFileName,
                                              const QString &pNewFileName)
{
    // Save the given file, if possible

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget != nullptr) {
        return simulationWidget->save(pNewFileName);
    }

    return false;
}

//==============================================================================

void SimulationExperimentViewWidget::fileOpened(const QString &pFileName)
{
    // Track the given file name

    mFileNames << pFileName;

    // Make sure that the GUI of our simulation widgets is up to date

    for (auto simulationWidget : mSimulationWidgets) {
        simulationWidget->updateGui(true);
    }
}

//==============================================================================

void SimulationExperimentViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been un/locked

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget != nullptr) {
        simulationWidget->filePermissionsChanged();
    }
}

//==============================================================================

void SimulationExperimentViewWidget::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so reload its simulation and consider the
    // file reloaded, but only if it has a corresponding widget that is
    // invisible

    SimulationExperimentViewSimulationWidget *crtSimulationWidget = simulationWidget(pFileName);

    if ((crtSimulationWidget != nullptr) && !crtSimulationWidget->isVisible()) {
        crtSimulationWidget->simulation()->reload();

        fileReloaded(pFileName);
    }
}

//==============================================================================

void SimulationExperimentViewWidget::fileReloaded(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been reloaded

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget != nullptr) {
        simulationWidget->fileReloaded();

        // Make sure that our simulation's contents' information GUI is up to
        // date
        // Note: this is, at least, necessary for our paramaters widget since we
        //       repopulate it, meaning that its columns' width will be reset...

        updateContentsInformationGui(simulationWidget);

        // Make sure that the GUI of our simulation widgets is up to date

        for (auto otherSimulationWidget : mSimulationWidgets) {
            otherSimulationWidget->updateGui(true);
        }
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

    if (simulationWidget != nullptr) {
        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }

    // Let our simulation widgets know that a file has been renamed

    for (auto otherSimulationWidget : mSimulationWidgets) {
        otherSimulationWidget->fileRenamed(pOldFileName, pNewFileName);
    }
}

//==============================================================================

void SimulationExperimentViewWidget::fileClosed(const QString &pFileName)
{
    // Stop tracking the given file name

    mFileNames.removeOne(pFileName);

    // Make sure that the GUI of our simulation widgets is up to date

    for (auto simulationWidget : mSimulationWidgets) {
        simulationWidget->updateGui(true);
    }
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

SimulationExperimentViewSimulationWidget * SimulationExperimentViewWidget::simulationWidget() const
{
    // Return our simulation widget

    return mSimulationWidget;
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

    if (simulationWidget != nullptr) {
        return simulationWidget->simulation();
    }

    return nullptr;
}

//==============================================================================

CellMLSupport::CellmlFileRuntime * SimulationExperimentViewWidget::runtime(const QString &pFileName) const
{
    // Return the runtime for the given file name

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget != nullptr) {
        return simulationWidget->simulation()->runtime();
    }

    return nullptr;
}

//==============================================================================

QWidget * SimulationExperimentViewWidget::widget(const QString &pFileName)
{
    // Return the requested (simulation) widget

    return simulationWidget(pFileName);
}

//==============================================================================

quint64 SimulationExperimentViewWidget::simulationResultsSize(const QString &pFileName) const
{
    // Return the results size for the given file name

    return mSimulationResultsSizes.value(pFileName);
}

//==============================================================================

void SimulationExperimentViewWidget::checkSimulationResults(const QString &pFileName,
                                                            SimulationExperimentViewSimulationWidget::Task pTask)
{
    // Make sure that we can still check results (i.e. we are not closing down
    // with some simulations still running)

    SimulationExperimentViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget == nullptr) {
        return;
    }

    // Make sure that our previous run, if any, is complete, if we are coming
    // here as a result of having added a new run

    SimulationSupport::Simulation *simulation = simulationWidget->simulation();
    int simulationRunsCount = simulation->runsCount();

    if (   (pTask == SimulationExperimentViewSimulationWidget::Task::AddRun)
        && (simulationRunsCount > 1)) {
        quint64 previousSimulationResultsSize = simulation->results()->size(simulationRunsCount-2);

        if (previousSimulationResultsSize != mSimulationResultsSizes.value(pFileName)) {
            for (auto currentSimulationWidget : mSimulationWidgets) {
                currentSimulationWidget->updateSimulationResults(simulationWidget,
                                                                 previousSimulationResultsSize,
                                                                 simulationRunsCount-2,
                                                                 SimulationExperimentViewSimulationWidget::Task::None);
            }
        }
    }

    // Update all of our simulation widgets' results, but only if needed
    // Note: to update only the given simulation widget's results is not enough
    //       since another simulation widget may have graphs that refer to the
    //       given simulation widget...

    quint64 simulationResultsSize = simulation->results()->size();

    if (   (pTask != SimulationExperimentViewSimulationWidget::Task::None)
        || (simulationResultsSize != mSimulationResultsSizes.value(pFileName))) {
        mSimulationResultsSizes.insert(pFileName, simulationResultsSize);

        for (auto currentSimulationWidget : mSimulationWidgets) {
            currentSimulationWidget->updateSimulationResults(simulationWidget,
                                                             simulationResultsSize,
                                                             simulationRunsCount-1,
                                                             pTask);
        }
    }

    // Ask to recheck our simulation widget's results, but only if its
    // simulation is still running

    if (   simulation->isRunning()
        || (simulationResultsSize != simulation->results()->size())) {
        QTimer::singleShot(0, this, std::bind(&SimulationExperimentViewWidget::checkSimulationResults,
                                              this, pFileName,
                                              SimulationExperimentViewSimulationWidget::Task::None));
    } else if (!simulation->isRunning() && !simulation->isPaused()) {
        // The simulation is over, so stop tracking the result's size and reset
        // the simulation progress of the given file

        mSimulationResultsSizes.remove(pFileName);

        simulationWidget->resetSimulationProgress();
    }
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

void SimulationExperimentViewWidget::collapsibleWidgetCollapsed(int pIndex,
                                                                bool pCollapsed)
{
    // One of the widgets in our collapsible widget has been collapsed or
    // expanded, so keep track of that fact

    mCollapsibleWidgetCollapsed[pIndex] = pCollapsed;
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelSettingsRequested()
{
    // Make sure that our graph panel settings are active and visible

    mSimulationWidget->contentsWidget()->informationWidget()->graphPanelAndGraphsWidget()->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::GraphPanel);
    mSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget()->setCollapsed(2, false);
}

//==============================================================================

void SimulationExperimentViewWidget::graphsSettingsRequested()
{
    // Make sure that our graphs settings are active and visible

    mSimulationWidget->contentsWidget()->informationWidget()->graphPanelAndGraphsWidget()->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::Graphs);
    mSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget()->setCollapsed(2, false);
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelGraphsModeChanged(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode pMode)
{
    // Keep track of the new graph panel / graphs mode

    mGraphPanelGraphsMode = pMode;
}

//==============================================================================

void SimulationExperimentViewWidget::simulationHeaderSectionResized(int pIndex,
                                                                    int pOldSize,
                                                                    int pNewSize)
{
    Q_UNUSED(pOldSize)

    // Keep track of the new column width

    mSimulationColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::solversHeaderSectionResized(int pIndex,
                                                                 int pOldSize,
                                                                 int pNewSize)
{
    Q_UNUSED(pOldSize)

    // Keep track of the new column width

    mSolversColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelHeaderSectionResized(int pIndex,
                                                                    int pOldSize,
                                                                    int pNewSize)
{
    Q_UNUSED(pOldSize)

    // Keep track of the new column width

    mGraphPanelColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphsHeaderSectionResized(int pIndex,
                                                                int pOldSize,
                                                                int pNewSize)
{
    Q_UNUSED(pOldSize)

    // Keep track of the new column width

    mGraphsColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::parametersHeaderSectionResized(int pIndex,
                                                                    int pOldSize,
                                                                    int pNewSize)
{
    Q_UNUSED(pOldSize)

    // Keep track of the new column width

    mParametersColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphPanelSectionExpanded(int pSection,
                                                               bool pExpanded)
{
    // Keep track of the section's expanded state

    mGraphPanelSectionsExpanded.insert(pSection, pExpanded);
}

//==============================================================================

void SimulationExperimentViewWidget::updateContentsInformationGui(SimulationExperimentViewSimulationWidget *pSimulationWidget)
{
    // Update our simualtion widget's GUI and that of its children
    // Note: for column widths, we set the last column width to zero to avoid
    //       potential issues, should the vertical scroll bar be visible...

    pSimulationWidget->setSizes(mSimulationWidgetSizes);

    SimulationExperimentViewContentsWidget *contentsWidget = pSimulationWidget->contentsWidget();

    contentsWidget->setSizes(mContentsWidgetSizes);

    SimulationExperimentViewInformationWidget *informationWidget = contentsWidget->informationWidget();

    for (int i = 0, iMax = mCollapsibleWidgetCollapsed.count(); i < iMax; ++i) {
        informationWidget->collapsibleWidget()->setCollapsed(i, mCollapsibleWidgetCollapsed[i]);
    }

    informationWidget->graphPanelAndGraphsWidget()->setMode(mGraphPanelGraphsMode);

    for (int i = 0, iMax = mSimulationColumnWidths.count(); i < iMax; ++i) {
        informationWidget->simulationWidget()->setColumnWidth(i, (i == iMax-1)?0:mSimulationColumnWidths[i]);
    }

    for (int i = 0, iMax = mSolversColumnWidths.count(); i < iMax; ++i) {
        informationWidget->solversWidget()->setColumnWidth(i, (i == iMax-1)?0:mSolversColumnWidths[i]);
    }

    for (int i = 0, iMax = mGraphPanelColumnWidths.count(); i < iMax; ++i) {
        informationWidget->graphPanelAndGraphsWidget()->setGraphPanelColumnWidth(i, (i == iMax-1)?0:mGraphPanelColumnWidths[i]);
    }

    for (int i = 0, iMax = mGraphsColumnWidths.count(); i < iMax; ++i) {
        informationWidget->graphPanelAndGraphsWidget()->setGraphsColumnWidth(i, (i == iMax-1)?0:mGraphsColumnWidths[i]);
    }

    for (int i = 0, iMax = mParametersColumnWidths.count(); i < iMax; ++i) {
        informationWidget->parametersWidget()->setColumnWidth(i, (i == iMax-1)?0:mParametersColumnWidths[i]);
    }

    QIntList sectionKeys = mGraphPanelSectionsExpanded.keys();

    for (auto section : sectionKeys) {
        informationWidget->graphPanelAndGraphsWidget()->setGraphPanelSectionExpanded(section, mGraphPanelSectionsExpanded.value(section));
    }
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
