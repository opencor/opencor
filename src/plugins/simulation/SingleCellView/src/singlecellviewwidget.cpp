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
// Single cell view widget
//==============================================================================

#include "collapsiblewidget.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewsimulationwidget.h"
#include "singlecellviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QLayout>
#include <QSettings>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewWidget::SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                           QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mSettingsGroup(QString()),
    mSimulationWidgetSizes(QIntList()),
    mContentsWidgetSizes(QIntList()),
    mCollapsibleWidgetCollapsed(QMap<int, bool>()),
    mSimulationWidgetColumnWidths(QIntList()),
    mSolversWidgetColumnWidths(QIntList()),
    mGraphsWidgetColumnWidths(QIntList()),
    mParametersWidgetColumnWidths(QIntList()),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SingleCellViewSimulationWidget *>()),
    mFileNames(QStringList()),
    mSimulationResultsSizes(QMap<QString, qulonglong>()),
    mSimulationCheckResults(QStringList())
{
}

//==============================================================================

static const auto SettingsSizes = QStringLiteral("Sizes");
static const auto SettingsContentsSizes = QStringLiteral("ContentsSizes");

//==============================================================================

void SingleCellViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the simulation widget's settings, but
    // mSimulationWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();

    // Retrieve the sizes of our simulation widget and of its contents widget

    QVariantList defaultContentsSizes = QVariantList() << 0.25*qApp->desktop()->screenGeometry().width()
                                                       << 0.75*qApp->desktop()->screenGeometry().width();

    mSimulationWidgetSizes = qVariantListToIntList(pSettings->value(SettingsSizes).toList());
    mContentsWidgetSizes = qVariantListToIntList(pSettings->value(SettingsContentsSizes, defaultContentsSizes).toList());
}

//==============================================================================

void SingleCellViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the sizes of our simulation widget and those of its
    // contents widget

    pSettings->setValue(SettingsSizes, qIntListToVariantList(mSimulationWidgetSizes));
    pSettings->setValue(SettingsContentsSizes, qIntListToVariantList(mContentsWidgetSizes));
}

//==============================================================================

void SingleCellViewWidget::retranslateUi()
{
    // Retranslate our simulation widgets

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets)
        simulationWidget->retranslateUi();
}

//==============================================================================

bool SingleCellViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mSimulationWidgets.contains(pFileName);
}

//==============================================================================

void SingleCellViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the simulation widget associated with the given file, if any

    SingleCellViewSimulationWidget *oldSimulationWidget = mSimulationWidget;

    mSimulationWidget = mSimulationWidgets.value(pFileName);

    if (!mSimulationWidget) {
        // No simulation widget exists for the given file, so create one

        mSimulationWidget = new SingleCellViewSimulationWidget(mPlugin, pFileName, this);

        // Keep track of our editing widget and add it to ourselves

        mSimulationWidgets.insert(pFileName, mSimulationWidget);

        layout()->addWidget(mSimulationWidget);

        // Load our simulation widget's settings and those of some of its
        // contents' children, if needed

        QSettings settings;

        settings.beginGroup(mSettingsGroup);
            mSimulationWidget->loadSettings(&settings);

            // Back up some settings, if this is our first simulation widget

            if (mSimulationWidgets.count() == 1)
                backupSettings(mSimulationWidget);
        settings.endGroup();

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

        connect(mSimulationWidget->contentsWidget()->informationWidget()->simulationWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(simulationWidgetHeaderSectionResized(const int &, const int &, const int &)));
        connect(mSimulationWidget->contentsWidget()->informationWidget()->solversWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(solversWidgetHeaderSectionResized(const int &, const int &, const int &)));
        connect(mSimulationWidget->contentsWidget()->informationWidget()->graphsWidget(), SIGNAL(headerSectionResized(int, int, int)),
                this, SLOT(graphsWidgetHeaderSectionResized(const int &, const int &, const int &)));
        connect(mSimulationWidget->contentsWidget()->informationWidget()->parametersWidget()->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(parametersWidgetHeaderSectionResized(const int &, const int &, const int &)));
    } else {
        // We already have a simulation widget, so just make sure that its GUI
        // is up to date

        mSimulationWidget->updateGui();
    }

    // Update our new simualtion widget and its children, if needed

    mSimulationWidget->setSizes(mSimulationWidgetSizes);
    mSimulationWidget->contentsWidget()->setSizes(mContentsWidgetSizes);

    // Set (restore) some settings, if this is not our first (created)
    // simulation widget (the first one is already properly set)

    if (mSimulationWidgets.count() > 1)
        restoreSettings(mSimulationWidget);

    // Hide our previous simulation widget and show our new one

    mSimulationWidget->show();

    if (oldSimulationWidget && (mSimulationWidget != oldSimulationWidget))
        oldSimulationWidget->hide();

    // Set our focus proxy to our 'new' simulation widget and make sure that the
    // latter immediately gets the focus

    setFocusProxy(mSimulationWidget);

    mSimulationWidget->setFocus();
}

//==============================================================================

void SingleCellViewWidget::finalize(const QString &pFileName)
{
    // Remove the simulation widget, should there be one for the given file

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        // There is a simulation widget for the given file name, so save its
        // settings and those of some of its contents' children, if needed

        QSettings settings;

        settings.beginGroup(mSettingsGroup);
            // Set (restore) some settings, if this is our last simulation
            // widget (this is in case we close OpenCOR after having modified a
            // simulation widget that is not the last one to be finalised)

            if (mSimulationWidgets.count() == 1)
                restoreSettings(simulationWidget);

            simulationWidget->saveSettings(&settings);
        settings.endGroup();

        // Finalise our simulation widget

        simulationWidget->finalize();

        // Now, we can delete it and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Finally, reset our memory of the simulation widget, if needed

        if (simulationWidget == mSimulationWidget)
            mSimulationWidget = 0;
    }
}

//==============================================================================

QIcon SingleCellViewWidget::fileTabIcon(const QString &pFileName) const
{
    // Return, if possible, the tab icon for the given file

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        return simulationWidget->fileTabIcon();
    } else {
        static const QIcon NoIcon = QIcon();

        return NoIcon;
    }
}

//==============================================================================

bool SingleCellViewWidget::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // Save the given file, if possible

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget)
        return simulationWidget->saveFile(pOldFileName, pNewFileName);
    else
        return false;
}

//==============================================================================

void SingleCellViewWidget::fileOpened(const QString &pFileName)
{
    // Track the given file name

    mFileNames << pFileName;

    // Let our simulation widgets know that a file has been opened

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->fileOpened(pFileName);
}

//==============================================================================

void SingleCellViewWidget::filePermissionsChanged(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been un/locked

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->filePermissionsChanged();
}

//==============================================================================

void SingleCellViewWidget::fileModified(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been modified

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileModified(pFileName);
}

//==============================================================================

void SingleCellViewWidget::fileReloaded(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been reloaded

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        simulationWidget->fileReloaded(pFileName);
}

//==============================================================================

void SingleCellViewWidget::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // Stop tracking the old given file name and track the given new one instead

    mFileNames.removeOne(pOldFileName);

    mFileNames << pNewFileName;

    // The given file has been renamed, so update our simulation widgets mapping

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pOldFileName);

    if (simulationWidget) {
        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }

    // Let our simulation widgets know that a file has been renamed

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->fileRenamed(pOldFileName, pNewFileName);
}

//==============================================================================

void SingleCellViewWidget::fileClosed(const QString &pFileName)
{
    // Stop tracking the given file name

    mFileNames.removeOne(pFileName);

    // Let our simulation widgets know that a file has been closed

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->fileClosed(pFileName);
}

//==============================================================================

QStringList SingleCellViewWidget::fileNames() const
{
    // Return our file names

    return mFileNames;
}

//==============================================================================

SingleCellViewSimulation * SingleCellViewWidget::simulation(const QString &pFileName) const
{
    // Return the simulation for the given file name

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        return simulationWidget->simulation();
    else
        return 0;
}

//==============================================================================

CellMLSupport::CellmlFileRuntime * SingleCellViewWidget::runtime(const QString &pFileName) const
{
    // Return the runtime for the given file name

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget)
        return simulationWidget->simulation()->runtime();
    else
        return 0;
}

//==============================================================================

qulonglong SingleCellViewWidget::simulationResultsSize(const QString &pFileName) const
{
    // Return the results size for the given file name

    return mSimulationResultsSizes.value(pFileName);
}

//==============================================================================

void SingleCellViewWidget::checkSimulationResults(const QString &pFileName,
                                                  const bool &pForceUpdateSimulationResults)
{
    // Make sure that we can still check results (i.e. we are not closing down
    // with some simulations still running)

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (!simulationWidget)
        return;

    // Update all of our simulation widgets' results, but only if needed
    // Note: to update only the given simulation widget's results is not enough
    //       since another simulation widget may have graphs that refer to the
    //       given simulation widget...

    SingleCellViewSimulation *simulation = simulationWidget->simulation();
    qulonglong simulationResultsSize = simulation->results()->size();

    if (   pForceUpdateSimulationResults
        || (simulationResultsSize != mSimulationResultsSizes.value(pFileName))) {
        mSimulationResultsSizes.insert(pFileName, simulationResultsSize);

        foreach (SingleCellViewSimulationWidget *currentSimulationWidget, mSimulationWidgets)
            currentSimulationWidget->updateSimulationResults(simulationWidget, simulationResultsSize);
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
        // The simulation is over, so stop tracking the result's size of the
        // given file

        mSimulationResultsSizes.remove(pFileName);
    }
}

//==============================================================================

void SingleCellViewWidget::callCheckSimulationResults()
{
    // Retrieve the simulation widget for which we want to call checkResults()
    // and then call checkResults() for it

    QString fileName = mSimulationCheckResults.first();

    mSimulationCheckResults.removeFirst();

    checkSimulationResults(fileName);
}

//==============================================================================

void SingleCellViewWidget::simulationWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our simulation widget has moved, so keep track of its new
    // sizes

    mSimulationWidgetSizes = pSizes;
}

//==============================================================================

void SingleCellViewWidget::contentsWidgetSplitterMoved(const QIntList &pSizes)
{
    // The splitter of our contents widget has moved, so keep track of its new
    // sizes

    mContentsWidgetSizes = pSizes;
}

//==============================================================================

void SingleCellViewWidget::collapsibleWidgetCollapsed(const int &pIndex,
                                                      const bool &pCollapsed)
{
    // One of the widgets in our collapsible widget has been collapsed or
    // expanded, so keep track of that fact

    mCollapsibleWidgetCollapsed.insert(pIndex, pCollapsed);
}

//==============================================================================

void SingleCellViewWidget::simulationWidgetHeaderSectionResized(const int &pIndex,
                                                                const int &pOldSize,
                                                                const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mSimulationWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::solversWidgetHeaderSectionResized(const int &pIndex,
                                                             const int &pOldSize,
                                                             const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mSolversWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::graphsWidgetHeaderSectionResized(const int &pIndex,
                                                            const int &pOldSize,
                                                            const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mGraphsWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::parametersWidgetHeaderSectionResized(const int &pIndex,
                                                                const int &pOldSize,
                                                                const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    mParametersWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::backupSettings(SingleCellViewSimulationWidget *pSimulationWidget)
{
    // Back up some of the given simulation's contents' children's settings

    Core::CollapsibleWidget *collapsibleWidget = pSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget();

    for (int i = 0, iMax = collapsibleWidget->count(); i < iMax; ++i)
        mCollapsibleWidgetCollapsed.insert(i, collapsibleWidget->isCollapsed(i));

    SingleCellViewInformationSimulationWidget *simulationWidget = pSimulationWidget->contentsWidget()->informationWidget()->simulationWidget();

    mSimulationWidgetColumnWidths = QIntList();

    for (int i = 0, iMax = simulationWidget->header()->count(); i < iMax; ++i)
        mSimulationWidgetColumnWidths << simulationWidget->columnWidth(i);

    SingleCellViewInformationSolversWidget *solversWidget = pSimulationWidget->contentsWidget()->informationWidget()->solversWidget();

    mSolversWidgetColumnWidths = QIntList();

    for (int i = 0, iMax = solversWidget->header()->count(); i < iMax; ++i)
        mSolversWidgetColumnWidths << solversWidget->columnWidth(i);

    SingleCellViewInformationGraphsWidget *graphsWidget = pSimulationWidget->contentsWidget()->informationWidget()->graphsWidget();

    mGraphsWidgetColumnWidths = QIntList();

    for (int i = 0, iMax = graphsWidget->headerCount(); i < iMax; ++i)
        mGraphsWidgetColumnWidths << graphsWidget->columnWidth(i);

    SingleCellViewInformationParametersWidget *parametersWidget = pSimulationWidget->contentsWidget()->informationWidget()->parametersWidget();

    mParametersWidgetColumnWidths = QIntList();

    for (int i = 0, iMax = parametersWidget->header()->count(); i < iMax; ++i)
        mParametersWidgetColumnWidths << parametersWidget->columnWidth(i);
}

//==============================================================================

void SingleCellViewWidget::restoreSettings(SingleCellViewSimulationWidget *pSimulationWidget)
{
    // Restore some of the given simulation's contents' children's settings

    Core::CollapsibleWidget *collapsibleWidget = pSimulationWidget->contentsWidget()->informationWidget()->collapsibleWidget();

    foreach (const int &index, mCollapsibleWidgetCollapsed.keys())
        collapsibleWidget->setCollapsed(index, mCollapsibleWidgetCollapsed.value(index));

    SingleCellViewInformationSimulationWidget *simulationWidget = pSimulationWidget->contentsWidget()->informationWidget()->simulationWidget();

    for (int i = 0, iMax = mSimulationWidgetColumnWidths.count(); i < iMax; ++i)
        simulationWidget->setColumnWidth(i, mSimulationWidgetColumnWidths[i]);

    SingleCellViewInformationSolversWidget *solversWidget = pSimulationWidget->contentsWidget()->informationWidget()->solversWidget();

    for (int i = 0, iMax = mSolversWidgetColumnWidths.count(); i < iMax; ++i)
        solversWidget->setColumnWidth(i, mSolversWidgetColumnWidths[i]);

    SingleCellViewInformationGraphsWidget *graphsWidget = pSimulationWidget->contentsWidget()->informationWidget()->graphsWidget();

    for (int i = 0, iMax = mGraphsWidgetColumnWidths.count(); i < iMax; ++i)
        graphsWidget->setColumnWidth(i, mGraphsWidgetColumnWidths[i]);

    SingleCellViewInformationParametersWidget *parametersWidget = pSimulationWidget->contentsWidget()->informationWidget()->parametersWidget();

    for (int i = 0, iMax = mParametersWidgetColumnWidths.count(); i < iMax; ++i)
        parametersWidget->setColumnWidth(i, mParametersWidgetColumnWidths[i]);
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
