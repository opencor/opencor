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
#include "simulationexperimentviewcontentswidget.h"
#include "simulationexperimentviewinformationgraphswidget.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewinformationsolverswidget.h"
#include "simulationexperimentviewinformationwidget.h"
#include "simulationexperimentviewplugin.h"
#include "simulationexperimentviewsimulation.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "simulationexperimentviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QHeaderView>
#include <QLayout>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "sbmlapibegin.h"
    #include "sbml/math/ASTNode.h"
#include "sbmlapiend.h"

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedPlot2D.h"
    #include "sedml/SedRepeatedTask.h"
    #include "sedml/SedUniformTimeCourse.h"
    #include "sedml/SedVectorRange.h"
#include "sedmlapiend.h"

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
    mSimulationWidgetColumnWidths(QIntList()),
    mSolversWidgetColumnWidths(QIntList()),
    mGraphsWidgetColumnWidths(QIntList()),
    mParametersWidgetColumnWidths(QIntList()),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SimulationExperimentViewSimulationWidget *>()),
    mFileNames(QStringList()),
    mSimulationResultsSizes(QMap<QString, qulonglong>()),
    mSimulationCheckResults(QStringList())
{
}

//==============================================================================

static const auto SettingsSizes = QStringLiteral("Sizes");
static const auto SettingsContentsSizes = QStringLiteral("ContentsSizes");
static const auto SettingsCollapsed = QStringLiteral("Collapsed");
static const auto SettingsSimulationColumnWidths = QStringLiteral("SimulationColumnWidths");
static const auto SettingsSolversColumnWidths = QStringLiteral("SolversColumnWidths");
static const auto SettingsGraphsColumnWidths = QStringLiteral("GraphsColumnWidths");
static const auto SettingsParametersColumnWidths = QStringLiteral("ParametersColumnWidths");

//==============================================================================

void SimulationExperimentViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the sizes of our simulation widget and of its contents widget

    QVariantList defaultContentsSizes = QVariantList() << 0.25*qApp->desktop()->screenGeometry().width()
                                                       << 0.75*qApp->desktop()->screenGeometry().width();

    mSimulationWidgetSizes = qVariantListToIntList(pSettings->value(SettingsSizes).toList());
    mContentsWidgetSizes = qVariantListToIntList(pSettings->value(SettingsContentsSizes, defaultContentsSizes).toList());

    // Retrieve the collapsed states of our collapsible widget

    QVariantList defaultCollapsed = QVariantList() << false << false << false;

    mCollapsibleWidgetCollapsed = qVariantListToBoolList(pSettings->value(SettingsCollapsed, defaultCollapsed).toList());

    // Retrieve the columns' width of our various property editors

    QVariantList defaultColumnWidths = QVariantList() << 100 << 100 << 100;

    mSimulationWidgetColumnWidths = qVariantListToIntList(pSettings->value(SettingsSimulationColumnWidths, defaultColumnWidths).toList());
    mSolversWidgetColumnWidths = qVariantListToIntList(pSettings->value(SettingsSolversColumnWidths, defaultColumnWidths).toList());
    mGraphsWidgetColumnWidths = qVariantListToIntList(pSettings->value(SettingsGraphsColumnWidths, defaultColumnWidths).toList());
    mParametersWidgetColumnWidths = qVariantListToIntList(pSettings->value(SettingsParametersColumnWidths, defaultColumnWidths).toList());
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

    // Keep track of the columns' width of our various property editors

    pSettings->setValue(SettingsSimulationColumnWidths, qIntListToVariantList(mSimulationWidgetColumnWidths));
    pSettings->setValue(SettingsSolversColumnWidths, qIntListToVariantList(mSolversWidgetColumnWidths));
    pSettings->setValue(SettingsGraphsColumnWidths, qIntListToVariantList(mGraphsWidgetColumnWidths));
    pSettings->setValue(SettingsParametersColumnWidths, qIntListToVariantList(mParametersWidgetColumnWidths));
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
    // Stop tracking changes in our 'old' simulation widget's property editors'
    // columns' width
    // Note: if we didn't do this then to set our 'new' simulation widget's
    //       property editors' columns' width would result in the sectionResized
    //       event being handled, which in turn would mess up our units column's
    //       width. Indeed, when our 'new' simulation widget's property editors'
    //       columns' width, QTreeView tries to optimise the columns' width. So,
    //       if we were to keep tracking those changes, we would end up with an
    //       width that is too big for our units column, resulting in a
    //       horizontal scrollbar being shown, which we don't want...

    SimulationExperimentViewSimulationWidget *oldSimulationWidget = mSimulationWidget;

    if (oldSimulationWidget) {
        disconnect(oldSimulationWidget->contentsWidget()->informationWidget()->simulationWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                   this, SLOT(simulationWidgetHeaderSectionResized(const int &, const int &, const int &)));
        disconnect(oldSimulationWidget->contentsWidget()->informationWidget()->solversWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                   this, SLOT(solversWidgetHeaderSectionResized(const int &, const int &, const int &)));
        disconnect(oldSimulationWidget->contentsWidget()->informationWidget()->graphsWidget(), SIGNAL(headerSectionResized(int, int, int)),
                   this, SLOT(graphsWidgetHeaderSectionResized(const int &, const int &, const int &)));
        disconnect(oldSimulationWidget->contentsWidget()->informationWidget()->parametersWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                   this, SLOT(parametersWidgetHeaderSectionResized(const int &, const int &, const int &)));
    }

    // Retrieve the simulation widget associated with the given file, if any

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (!mSimulationWidget) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SimulationExperimentViewSimulationWidget(mPlugin, this, pFileName, this);

        // Keep track of our editing widget

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        // Initialise our simulation widget

        mSimulationWidget->initialize();

        // Keep track of various things related our simulation widget and its
        // children

        connect(mSimulationWidget, SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(simulationWidgetSplitterMoved(const QIntList &)));

        connect(mSimulationWidget->contentsWidget(), SIGNAL(splitterMoved(const QIntList &)),
                this, SLOT(contentsWidgetSplitterMoved(const QIntList &)));

        connect(mSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget(), SIGNAL(collapsed(const int &, const bool &)),
                this, SLOT(collapsibleWidgetCollapsed(const int &, const bool &)));
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

    // Keep track of changes in our 'new' simulation widget's property editors'
    // columns' width

    connect(mSimulationWidget->contentsWidget()->informationWidget()->simulationWidget()->header(), SIGNAL(sectionResized(int, int, int)),
            this, SLOT(simulationWidgetHeaderSectionResized(const int &, const int &, const int &)));
    connect(mSimulationWidget->contentsWidget()->informationWidget()->solversWidget()->header(), SIGNAL(sectionResized(int, int, int)),
            this, SLOT(solversWidgetHeaderSectionResized(const int &, const int &, const int &)));
    connect(mSimulationWidget->contentsWidget()->informationWidget()->graphsWidget(), SIGNAL(headerSectionResized(int, int, int)),
            this, SLOT(graphsWidgetHeaderSectionResized(const int &, const int &, const int &)));
    connect(mSimulationWidget->contentsWidget()->informationWidget()->parametersWidget()->header(), SIGNAL(sectionResized(int, int, int)),
            this, SLOT(parametersWidgetHeaderSectionResized(const int &, const int &, const int &)));

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

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SimulationExperimentViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
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

SimulationExperimentViewSimulation * SimulationExperimentViewWidget::simulation(const QString &pFileName) const
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

    SimulationExperimentViewSimulation *simulation = simulationWidget->simulation();
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

void SimulationExperimentViewWidget::simulationWidgetHeaderSectionResized(const int &pIndex,
                                                                          const int &pOldSize,
                                                                          const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mSimulationWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::solversWidgetHeaderSectionResized(const int &pIndex,
                                                                       const int &pOldSize,
                                                                       const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mSolversWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::graphsWidgetHeaderSectionResized(const int &pIndex,
                                                                      const int &pOldSize,
                                                                      const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<SimulationExperimentViewInformationGraphsWidget *>(sender())->isVisible())
        mGraphsWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::parametersWidgetHeaderSectionResized(const int &pIndex,
                                                                          const int &pOldSize,
                                                                          const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mParametersWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SimulationExperimentViewWidget::updateContentsInformationGui(SimulationExperimentViewSimulationWidget *pSimulationWidget)
{
    // Update some of our simulation's contents' information GUI

    for (int i = 0, iMax = mCollapsibleWidgetCollapsed.count(); i < iMax; ++i)
        pSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget()->setCollapsed(i, mCollapsibleWidgetCollapsed[i]);

    for (int i = 0, iMax = mSimulationWidgetColumnWidths.count(); i < iMax; ++i)
        pSimulationWidget->contentsWidget()->informationWidget()->simulationWidget()->setColumnWidth(i, mSimulationWidgetColumnWidths[i]);

    for (int i = 0, iMax = mSolversWidgetColumnWidths.count(); i < iMax; ++i)
        pSimulationWidget->contentsWidget()->informationWidget()->solversWidget()->setColumnWidth(i, mSolversWidgetColumnWidths[i]);

    for (int i = 0, iMax = mGraphsWidgetColumnWidths.count(); i < iMax; ++i)
        pSimulationWidget->contentsWidget()->informationWidget()->graphsWidget()->setColumnWidth(i, mGraphsWidgetColumnWidths[i]);

    for (int i = 0, iMax = mParametersWidgetColumnWidths.count(); i < iMax; ++i)
        pSimulationWidget->contentsWidget()->informationWidget()->parametersWidget()->setColumnWidth(i, mParametersWidgetColumnWidths[i]);
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
