/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Single Cell view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "collapsiblewidget.h"
#include "combinefilemanager.h"
#include "filemanager.h"
#include "sedmlfilemanager.h"
#include "singlecellviewcontentswidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewinformationwidget.h"
#include "singlecellviewplugin.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewsimulationwidget.h"
#include "singlecellviewwidget.h"

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
namespace SingleCellView {

//==============================================================================

SingleCellViewWidget::SingleCellViewWidget(SingleCellViewPlugin *pPlugin,
                                           QWidget *pParent) :
    ViewWidget(pParent),
    mPlugin(pPlugin),
    mSimulationWidgetSizes(QIntList()),
    mContentsWidgetSizes(QIntList()),
    mCollapsibleWidgetCollapsed(QBoolList()),
    mSimulationWidgetColumnWidths(QIntList()),
    mSolversWidgetColumnWidths(QIntList()),
    mGraphsWidgetColumnWidths(QIntList()),
    mParametersWidgetColumnWidths(QIntList()),
    mSimulationWidget(0),
    mSimulationWidgets(QMap<QString, SingleCellViewSimulationWidget *>()),
    mFileNames(QStringList()),
    mSimulationResultsSizes(QMap<QString, qulonglong>()),
    mSimulationCheckResults(QStringList()),
    mLocallyManagedCellmlFiles(QMap<QString, QString>())
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

void SingleCellViewWidget::loadSettings(QSettings *pSettings)
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

void SingleCellViewWidget::saveSettings(QSettings *pSettings) const
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

void SingleCellViewWidget::retranslateUi()
{
    // Retranslate our simulation widgets

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets)
        simulationWidget->retranslateUi();
}

//==============================================================================

bool SingleCellViewWidget::isIndirectRemoteFile(const QString &pFileName)
{
    // Return whether the given file is an indirect remote file by retrieving
    // its details

    Core::File *file = Core::FileManager::instance()->file(pFileName);

    if (file->isLocal()) {
        CellMLSupport::CellmlFile *cellmlFile = 0;
        SEDMLSupport::SedmlFile *sedmlFile = 0;
        COMBINESupport::CombineArchive *combineArchive = 0;
        FileType fileType;
        SEDMLSupport::SedmlFileIssues sedmlFileIssues;
        COMBINESupport::CombineArchiveIssues combineArchiveIssues;
        bool res = false;

        retrieveFileDetails(pFileName, cellmlFile, sedmlFile, combineArchive,
                            fileType, sedmlFileIssues, combineArchiveIssues,
                            &res);

        if (fileType != CellmlFile)
            delete cellmlFile;

        if (fileType != SedmlFile)
            delete sedmlFile;

        return res;
    } else {
        return false;
    }
}

//==============================================================================

void SingleCellViewWidget::initialize(const QString &pFileName)
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

    SingleCellViewSimulationWidget *oldSimulationWidget = mSimulationWidget;

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

        mSimulationWidget = new SingleCellViewSimulationWidget(mPlugin, pFileName, this);

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

void SingleCellViewWidget::finalize(const QString &pFileName)
{
    // Remove the simulation widget, should there be one for the given file

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        // Finalise our simulation widget

        simulationWidget->finalize();

        // Now, we can delete it and remove it from our list

        delete simulationWidget;

        mSimulationWidgets.remove(pFileName);

        // Next, we reset our memory of the simulation widget, if needed

        if (simulationWidget == mSimulationWidget)
            mSimulationWidget = 0;

        // Finally, we ask our file manager to stop managing our locally managed
        // CellML file, if any

        QString cellmlFileName = mLocallyManagedCellmlFiles.value(pFileName);

        if (!cellmlFileName.isEmpty()) {
            Core::FileManager::instance()->unmanage(cellmlFileName);

            mLocallyManagedCellmlFiles.remove(pFileName);
        }
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
        return simulationWidget->save(pNewFileName);
    else
        return false;
}

//==============================================================================

void SingleCellViewWidget::fileOpened(const QString &pFileName)
{
    // Track the given file name

    mFileNames << pFileName;

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
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
        simulationWidget->fileModified();
}

//==============================================================================

void SingleCellViewWidget::fileReloaded(const QString &pFileName)
{
    // Let the simulation widget, if any, associated with the given file name
    // know that a file has been reloaded

    SingleCellViewSimulationWidget *simulationWidget = mSimulationWidgets.value(pFileName);

    if (simulationWidget) {
        simulationWidget->fileReloaded();

        // Make sure that our simulation's contents' information GUI is up to
        // date
        // Note: this is, at least, necessary for our paramaters widget since we
        //       repopulate it, meaning that its columns' width will be reset...

        updateContentsInformationGui(simulationWidget);

        // Make sure that the GUI of our simulation widgets is up to date

        foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
            simulationWidget->updateGui(true);
    }
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
        simulationWidget->setFileName(pNewFileName);

        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
}

//==============================================================================

void SingleCellViewWidget::fileClosed(const QString &pFileName)
{
    // Stop tracking the given file name

    mFileNames.removeOne(pFileName);

    // Make sure that the GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui(true);
}

//==============================================================================

QStringList SingleCellViewWidget::fileNames() const
{
    // Return our file names

    return mFileNames;
}

//==============================================================================

SingleCellViewSimulationWidget * SingleCellViewWidget::simulationWidget(const QString &pFileName) const
{
    // Return the requested simulation widget

    return mSimulationWidgets.value(pFileName);
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

QWidget * SingleCellViewWidget::widget(const QString &pFileName)
{
    // Return the requested (simulation) widget

    return simulationWidget(pFileName);
}

//==============================================================================

qulonglong SingleCellViewWidget::simulationResultsSize(const QString &pFileName) const
{
    // Return the results size for the given file name

    return mSimulationResultsSizes.value(pFileName);
}

//==============================================================================

void SingleCellViewWidget::checkSimulationResults(const QString &pFileName,
                                                  const bool &pClearGraphs)
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

    if (   pClearGraphs
        || (simulationResultsSize != mSimulationResultsSizes.value(pFileName))) {
        mSimulationResultsSizes.insert(pFileName, simulationResultsSize);

        foreach (SingleCellViewSimulationWidget *currentSimulationWidget, mSimulationWidgets)
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

    mCollapsibleWidgetCollapsed[pIndex] = pCollapsed;
}

//==============================================================================

void SingleCellViewWidget::simulationWidgetHeaderSectionResized(const int &pIndex,
                                                                const int &pOldSize,
                                                                const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mSimulationWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::solversWidgetHeaderSectionResized(const int &pIndex,
                                                             const int &pOldSize,
                                                             const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mSolversWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::graphsWidgetHeaderSectionResized(const int &pIndex,
                                                            const int &pOldSize,
                                                            const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<SingleCellViewInformationGraphsWidget *>(sender())->isVisible())
        mGraphsWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::parametersWidgetHeaderSectionResized(const int &pIndex,
                                                                const int &pOldSize,
                                                                const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Keep track of the new column width

    if (qobject_cast<QHeaderView *>(sender())->isVisible())
        mParametersWidgetColumnWidths[pIndex] = pNewSize;
}

//==============================================================================

void SingleCellViewWidget::updateContentsInformationGui(SingleCellViewSimulationWidget *pSimulationWidget)
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

bool SingleCellViewWidget::sedmlAlgorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                                   SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
    // Make sure that we have an algorithm

    if (!pSedmlAlgorithm) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one or two simulations with an algorithm are supported"));

        return false;
    }

    // Make sure that the given algorithm relies on an algorithm that we support

    SolverInterface *usedSolverInterface = 0;
    QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getKisaoID());

    foreach (SolverInterface *solverInterface, mPlugin->solverInterfaces()) {
        if (!solverInterface->id(kisaoId).compare(solverInterface->solverName())) {
            usedSolverInterface = solverInterface;

            break;
        }
    }

    if (!usedSolverInterface) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("unsupported algorithm (%1)").arg(kisaoId));

        return false;
    }

    // Make sure that the algorithm parameters are also supported

    for (int i = 0, iMax = pSedmlAlgorithm->getNumAlgorithmParameters(); i < iMax; ++i) {
        QString kisaoId = QString::fromStdString(pSedmlAlgorithm->getAlgorithmParameter(i)->getKisaoID());
        QString id = usedSolverInterface->id(kisaoId);

        if (id.isEmpty() || !id.compare(usedSolverInterface->solverName())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("unsupported algorithm parameter (%1)").arg(kisaoId));

            return false;
        }
    }

    // Make sure that the annotation, if any, contains at least the kind of
    // information we would expect

    libsbml::XMLNode *annotation = pSedmlAlgorithm->getAnnotation();

    if (annotation) {
        for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
            const XMLNode &node = annotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::SolverProperties)) {
                continue;
            }

            bool validSolverProperties = true;

            for (uint j = 0, jMax = node.getNumChildren(); j < jMax; ++j) {
                const XMLNode &solverPropertyNode = node.getChild(j);

                if (   QString::fromStdString(solverPropertyNode.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    || QString::fromStdString(solverPropertyNode.getName()).compare(SEDMLSupport::SolverProperty)) {
                    continue;
                }

                int idIndex = solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyId.toStdString());
                int valueIndex = solverPropertyNode.getAttrIndex(SEDMLSupport::SolverPropertyValue.toStdString());

                if (   (idIndex == -1) || (valueIndex == -1)
                    || solverPropertyNode.getAttrValue(idIndex).empty()
                    || solverPropertyNode.getAttrValue(valueIndex).empty()) {
                    validSolverProperties = false;

                    break;
                }
            }

            if (!validSolverProperties) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("incomplete algorithm annotation (missing algorithm property information)"));

                return false;
            }
        }
    }

    return true;
}

//==============================================================================

bool SingleCellViewWidget::sedmlFileSupported(SEDMLSupport::SedmlFile *pSedmlFile,
                                              SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
    // Make sure that there is only one model

    libsedml::SedDocument *sedmlDocument = pSedmlFile->sedmlDocument();

    if (sedmlDocument->getNumModels() != 1) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one model are supported"));

        return false;
    }

    // Make sure that the model is of CellML type

    libsedml::SedModel *model = sedmlDocument->getModel(0);
    QString language = QString::fromStdString(model->getLanguage());

    if (   language.compare(SEDMLSupport::Language::Cellml)
        && language.compare(SEDMLSupport::Language::Cellml_1_0)
        && language.compare(SEDMLSupport::Language::Cellml_1_1)) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with a CellML file are supported"));

        return false;
    }

    // Make sure that there is either one or two simulations

    int nbOfSimulations = sedmlDocument->getNumSimulations();

    if ((nbOfSimulations != 1) && (nbOfSimulations != 2)) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one or two simulations are supported"));

        return false;
    }

    // Make sure that the first simulation is a uniform time course simulation

    libsedml::SedSimulation *firstSimulation = sedmlDocument->getSimulation(0);

    if (firstSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_UNIFORMTIMECOURSE) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with a uniform time course as a (first) simulation are supported"));

        return false;
    }

    // Make sure that the initial time and output start time are the same, that
    // the output start time and output end time are different, and that the
    // number of points is greater than zero

    libsedml::SedUniformTimeCourse *uniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(firstSimulation);
    double initialTime = uniformTimeCourse->getInitialTime();
    double outputStartTime = uniformTimeCourse->getOutputStartTime();
    double outputEndTime = uniformTimeCourse->getOutputEndTime();
    int nbOfPoints = uniformTimeCourse->getNumberOfPoints();

    if (initialTime != outputStartTime) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with the same values for initialTime and outputStartTime are supported"));

        return false;
    }

    if (outputStartTime == outputEndTime) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                         tr("the values for outputStartTime and outputEndTime must be different"));

        return false;
    }

    if (nbOfPoints <= 0) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                         tr("the value for numberOfPoints must be greater than zero"));

        return false;
    }

    // Make sure that the algorithm used for the first simulation is supported

    if (!sedmlAlgorithmSupported(firstSimulation->getAlgorithm(), pSedmlFileIssues))
        return false;

    // Make sure that the annotation, if any, contains at least the kind of
    // information we would expect

    libsbml::XMLNode *firstSimulationAnnotation = firstSimulation->getAnnotation();

    if (firstSimulationAnnotation) {
        bool hasNlaSolver = false;

        for (uint i = 0, iMax = firstSimulationAnnotation->getNumChildren(); i < iMax; ++i) {
            const libsbml::XMLNode &node = firstSimulationAnnotation->getChild(i);

            if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                || QString::fromStdString(node.getName()).compare(SEDMLSupport::NlaSolver)) {
                continue;
            }

            int nameIndex = node.getAttrIndex(SEDMLSupport::NlaSolverName.toStdString());

            if ((nameIndex != -1) && !node.getAttrValue(nameIndex).empty()) {
                if (hasNlaSolver) {
                    pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                     tr("only one NLA solver is allowed"));

                    return false;
                } else {
                    hasNlaSolver = true;
                }
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("incomplete simulation annotation (missing NLA solver name)"));

                return false;
            }
        }
    }

    // Check whether there is a second simulation

    libsedml::SedSimulation *secondSimulation = sedmlDocument->getSimulation(1);

    if (secondSimulation) {
        // Make sure that the second simulation is a one-step simulation

        if (secondSimulation->getTypeCode() != libsedml::SEDML_SIMULATION_ONESTEP) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with a one-step as a second simulation are supported"));

            return false;
        }

        // Make sure that its step is greater than zero

        if (static_cast<libsedml::SedOneStep *>(secondSimulation)->getStep() <= 0) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("the value for step must be greater than zero"));

            return false;
        }

        // Make sure that its algorithm and annotation, if any, is the same as
        // for the first simulation

        std::stringstream firstStream;
        std::stringstream secondStream;
        libsbml::XMLOutputStream firstXmlStream(firstStream);
        libsbml::XMLOutputStream secondXmlStream(secondStream);

        firstSimulation->getAlgorithm()->write(firstXmlStream);

        if (secondSimulation->getAlgorithm())
            secondSimulation->getAlgorithm()->write(secondXmlStream);

        libsbml::XMLNode *secondSimulationAnnotation = secondSimulation->getAnnotation();

        if (firstSimulationAnnotation)
            firstSimulationAnnotation->write(firstXmlStream);

        if (secondSimulationAnnotation)
            secondSimulationAnnotation->write(secondXmlStream);

        if (firstStream.str().compare(secondStream.str())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with two simulations with the same algorithm are supported"));

            return false;
        }
    }

    // Make sure that we have only one repeated task, which aim is to execute
    // each simulation (using a sub-task) once

    uint totalNbOfTasks = secondSimulation?3:2;

    if (sedmlDocument->getNumTasks() != totalNbOfTasks) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    libsedml::SedRepeatedTask *repeatedTask = 0;

    bool repeatedTaskOk = false;
    std::string repeatedTaskFirstSubTaskId = std::string();
    std::string repeatedTaskSecondSubTaskId = std::string();

    bool firstSubTaskOk = false;
    std::string firstSubTaskId = std::string();

    bool secondSubTaskOk = false;
    std::string secondSubTaskId = std::string();

    for (uint i = 0; i < totalNbOfTasks; ++i) {
        libsedml::SedTask *task = sedmlDocument->getTask(i);

        if (task->getTypeCode() == libsedml::SEDML_TASK_REPEATEDTASK) {
            // Make sure that the repeated task asks for the model to be reset,
            // that it has one range, no task change and one/two sub-task/s

            repeatedTask = static_cast<libsedml::SedRepeatedTask *>(task);

            if (    repeatedTask->getResetModel()
                &&  (repeatedTask->getNumRanges() == 1)
                && !repeatedTask->getNumTaskChanges()
                &&  (repeatedTask->getNumSubTasks() == totalNbOfTasks-1)) {
                // Make sure that the range is a vector range and that it's the
                // one referenced in the repeated task

                libsedml::SedRange *range = repeatedTask->getRange(0);

                if (    (range->getTypeCode() == libsedml::SEDML_RANGE_VECTORRANGE)
                    && !repeatedTask->getRangeId().compare(range->getId())) {
                    // Make sure that the vector range has one value that is
                    // equal to 1

                    libsedml::SedVectorRange *vectorRange = static_cast<libsedml::SedVectorRange *>(range);

                    if (   (vectorRange->getNumValues() == 1)
                        && (vectorRange->getValues().front() == 1)) {
                        // Make sure that the one/two sub-tasks have the correct
                        // order and retrieve their id

                        for (uint i = 0, iMax = totalNbOfTasks-1; i < iMax; ++i) {
                            libsedml::SedSubTask *subTask = repeatedTask->getSubTask(i);

                            if (subTask->getOrder() == 1)
                                repeatedTaskFirstSubTaskId = subTask->getTask();
                            else if (subTask->getOrder() == 2)
                                repeatedTaskSecondSubTaskId = subTask->getTask();
                        }

                        repeatedTaskOk = true;
                    }
                }
            }
        } else if (task->getTypeCode() == libsedml::SEDML_TASK) {
            // Make sure the sub-task references the correct model and
            // simulation

            if (   !task->getModelReference().compare(model->getId())
                && !task->getSimulationReference().compare(firstSimulation->getId())) {
                firstSubTaskOk = true;
                firstSubTaskId = task->getId();
            } else if (   secondSimulation
                       && !task->getModelReference().compare(model->getId())
                       && !task->getSimulationReference().compare(secondSimulation->getId())) {
                secondSubTaskOk = true;
                secondSubTaskId = task->getId();
            }
        }
    }

    if (   !repeatedTaskOk
        || !firstSubTaskOk || repeatedTaskFirstSubTaskId.compare(firstSubTaskId)
        || (     secondSimulation
            && (!secondSubTaskOk || repeatedTaskSecondSubTaskId.compare(secondSubTaskId)))) {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files that execute one or two simulations once are supported"));

        return false;
    }

    // Make sure that all the data generators have one variable that references
    // the repeated task, that follows the correct CellML format for their
    // target (and OpenCOR format for their degree, if any), and that is not
    // modified

    for (uint i = 0, iMax = sedmlDocument->getNumDataGenerators(); i < iMax; ++i) {
        libsedml::SedDataGenerator *dataGenerator = sedmlDocument->getDataGenerator(i);

        if ((dataGenerator->getNumVariables() != 1) || dataGenerator->getNumParameters()) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable are supported"));

            return false;
        }

        libsedml::SedVariable *variable = dataGenerator->getVariable(0);

        if (variable->getSymbol().size() || variable->getModelReference().size()) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a target and a task reference are supported"));

            return false;
        }

        if (variable->getTaskReference().compare(repeatedTask->getId())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a reference to a repeated task are supported"));

            return false;
        }

        static const QRegularExpression TargetStartRegEx  = QRegularExpression("^\\/cellml:model\\/cellml:component\\[@name='");
        static const QRegularExpression TargetMiddleRegEx = QRegularExpression("']\\/cellml:variable\\[@name='");
        static const QRegularExpression TargetEndRegEx    = QRegularExpression("'\\]$");

        bool referencingCellmlVariable = false;
        QString target = QString::fromStdString(variable->getTarget());

        if (target.contains(TargetStartRegEx) && target.contains(TargetEndRegEx)) {
            static const QString Separator = "|";

            target.remove(TargetStartRegEx);
            target.replace(TargetMiddleRegEx, Separator);
            target.remove(TargetEndRegEx);

            QStringList identifiers = target.split(Separator);

            if (identifiers.count() == 2) {
                static const QRegularExpression IdentifierRegEx = QRegularExpression("^[[:alpha:]_][[:alnum:]_]*$");

                QString componentName = identifiers.first();
                QString variableName = identifiers.last();

                referencingCellmlVariable =    IdentifierRegEx.match(componentName).hasMatch()
                                            && IdentifierRegEx.match(variableName).hasMatch();
            }
        }

        if (!referencingCellmlVariable) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable with a reference to a CellML variable are supported"));

            return false;
        }

        libsbml::XMLNode *annotation = variable->getAnnotation();

        if (annotation) {
            for (uint i = 0, iMax = annotation->getNumChildren(); i < iMax; ++i) {
                const XMLNode &node = annotation->getChild(i);

                if (   QString::fromStdString(node.getURI()).compare(SEDMLSupport::OpencorNamespace)
                    || QString::fromStdString(node.getName()).compare(SEDMLSupport::VariableDegree)) {
                    continue;
                }

                bool validVariableDegree = false;

                if (node.getNumChildren() == 1) {
                    bool conversionOk;
                    int variableDegree = QString::fromStdString(node.getChild(0).getCharacters()).toInt(&conversionOk);

                    validVariableDegree = conversionOk && (variableDegree >= 0);
                }

                if (!validVariableDegree) {
                    pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                     tr("only SED-ML files with data generators for one variable that is derived or not are supported"));

                    return false;
                }
            }
        }

        const libsbml::ASTNode *mathNode = dataGenerator->getMath();

        if (   (mathNode->getType() != libsbml::AST_NAME)
            || variable->getId().compare(mathNode->getName())) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with data generators for one variable that is not modified are supported"));

            return false;
        }
    }

    // Make sure that all the outputs are 2D outputs

    for (uint i = 0, iMax = sedmlDocument->getNumOutputs(); i < iMax; ++i) {
        libsedml::SedOutput *output = sedmlDocument->getOutput(i);

        if (output->getTypeCode() != libsedml::SEDML_OUTPUT_PLOT2D) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with 2D outputs are supported"));

            return false;
        }

        // Make sure that the curves reference listed data generators and don't
        // use logarithmic axes

        libsedml::SedPlot2D *plot = static_cast<libsedml::SedPlot2D *>(output);

        for (uint j = 0, jMax = plot->getNumCurves(); j < jMax; ++j) {
            libsedml::SedCurve *curve = plot->getCurve(j);

            if (curve->getLogX() || curve->getLogY()) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("only SED-ML files with linear 2D outputs are supported"));

                return false;
            }

            if (   !sedmlDocument->getDataGenerator(curve->getXDataReference())
                || !sedmlDocument->getDataGenerator(curve->getYDataReference())) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("a curve must reference existing data generators"));

                return false;
            }
        }
    }

    return true;
}

//==============================================================================

bool SingleCellViewWidget::combineArchiveSupported(COMBINESupport::CombineArchive *pCombineArchive,
                                                   COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const
{
    // Load and make sure that our COMBINE archive is valid

    if (   !pCombineArchive->load()
        || !pCombineArchive->isValid(pCombineArchiveIssues)) {
        return false;
    }

    // Make sure that there is only one master file in our COMBINE archive

    if (pCombineArchive->masterFiles().count() != 1) {
        pCombineArchiveIssues << COMBINESupport::CombineArchiveIssue(COMBINESupport::CombineArchiveIssue::Information,
                                                                     tr("only COMBINE archives with one master file are supported"));

        return false;
    }

    return true;
}

//==============================================================================

void SingleCellViewWidget::retrieveCellmlFile(const QString &pFileName,
                                              CellMLSupport::CellmlFile *&pCellmlFile,
                                              SEDMLSupport::SedmlFile *pSedmlFile,
                                              const FileType &pFileType,
                                              SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                                              bool *pIsDirectOrIndirectRemoteFile)
{
    // Make sure that we support our SED-ML file

    if (!sedmlFileSupported(pSedmlFile, pSedmlFileIssues))
        return;

    // Retrieve the source of the CellML file, if any, referenced in our SED-ML
    // file

    QString modelSource = QString::fromStdString(pSedmlFile->sedmlDocument()->getModel(0)->getSource());

    // Check whether we are dealing with a local file (which location is
    // relative to that of our SED-ML file) or a remote file

    if (pIsDirectOrIndirectRemoteFile) {
        // We only want to determine whether we are dealing with a local file or
        // a remote one

        QString dummy;

        Core::checkFileNameOrUrl(modelSource, *pIsDirectOrIndirectRemoteFile, dummy);

        *pIsDirectOrIndirectRemoteFile = !*pIsDirectOrIndirectRemoteFile;
        // Note: since Core::checkFileNameOrUrl() tells us whether we are
        //       dealing with a local file...
    } else {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString url = fileManagerInstance->file(pFileName)->url();
        bool isLocalFile;
        QString dummy;

        Core::checkFileNameOrUrl(modelSource, isLocalFile, dummy);

        if (isLocalFile && url.isEmpty()) {
            // By default, our model source refers to a file name relative to
            // our SED-ML file

            QString cellmlFileName = Core::nativeCanonicalFileName(QFileInfo(pSedmlFile->fileName()).path()+QDir::separator()+modelSource);

#ifdef Q_OS_WIN
            // On Windows, if our model source exists, it means that it refers
            // to a file on a different drive rather than to a file name
            // relative to our SED-ML file

            if (QFile::exists(modelSource))
                cellmlFileName = modelSource;
#endif

            if (QFile::exists(cellmlFileName)) {
                pCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);

                // If possible, set our CellML file and its dependencies, if
                // any, as dependencies for our SED-ML file

                if (pFileType == SedmlFile) {
                    Core::FileManager::instance()->setDependencies(pFileName,
                                                                   QStringList() << pCellmlFile->fileName()
                                                                                 << pCellmlFile->dependencies(true));
                }
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("%1 could not be found").arg(modelSource));
            }
        } else {
            // Handle the case where our model source is a relative remote file

            static const QRegularExpression FileNameRegEx = QRegularExpression("/[^/]*$");

            if (isLocalFile)
                modelSource = url.remove(FileNameRegEx)+"/"+modelSource;

            // Retrieve the contents of our model source

            QString fileContents;
            QString errorMessage;

            if (Core::readFileContentsFromUrlWithBusyWidget(modelSource, fileContents, &errorMessage)) {
                // Save the contents of our model source to a local file and use
                // that to create a CellML file object after having asked our
                // file manager to manage it (so that CellML 1.1 files can be
                // properly instantiated)
                // Note: we also keep track of our model source's local file
                //       since we will need to unmanage it when closing this
                //       file...

                QString cellmlFileName = Core::temporaryFileName();

                if (Core::writeFileContentsToFile(cellmlFileName, fileContents)) {
                    fileManagerInstance->manage(cellmlFileName, Core::File::Remote, modelSource);

                    pCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);

                    mLocallyManagedCellmlFiles.insert(pFileName, cellmlFileName);
                } else {
                    pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                     tr("%1 could not be saved").arg(modelSource));
                }
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("%1 could not be retrieved (%2)").arg(modelSource, Core::formatMessage(errorMessage)));
            }
        }
    }
}

//==============================================================================

void SingleCellViewWidget::retrieveSedmlFile(SEDMLSupport::SedmlFile *&pSedmlFile,
                                             COMBINESupport::CombineArchive *pCombineArchive,
                                             COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues)
{
    // Make sure that we support our COMBINE archive

    if (!combineArchiveSupported(pCombineArchive, pCombineArchiveIssues))
        return;

    // Create a SED-ML file object for our COMBINE archive's master file

    pSedmlFile = new SEDMLSupport::SedmlFile(pCombineArchive->masterFiles().first().fileName());
}

//==============================================================================

void SingleCellViewWidget::retrieveFileDetails(const QString &pFileName,
                                               CellMLSupport::CellmlFile *&pCellmlFile,
                                               SEDMLSupport::SedmlFile *&pSedmlFile,
                                               COMBINESupport::CombineArchive *&pCombineArchive,
                                               FileType &pFileType,
                                               SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                                               COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues,
                                               bool *pIsDirectOrIndirectRemoteFile)
{
    // Determine the type of file we are dealing with

    pCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
    pSedmlFile = pCellmlFile?0:SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName);
    pCombineArchive = pSedmlFile?0:COMBINESupport::CombineFileManager::instance()->combineArchive(pFileName);

    pFileType = pCellmlFile?CellmlFile:pSedmlFile?SedmlFile:CombineArchive;

    // In the case of a COMBINE archive, we need to retrieve the corresponding
    // SED-ML file while, in the case of a SED-ML file, we need to retrieve the
    // corresponding CellML file

    pSedmlFileIssues.clear();
    pCombineArchiveIssues.clear();

    if (pCombineArchive)
        retrieveSedmlFile(pSedmlFile, pCombineArchive, pCombineArchiveIssues);

    if (pSedmlFile) {
        retrieveCellmlFile(pFileName, pCellmlFile, pSedmlFile, pFileType,
                           pSedmlFileIssues, pIsDirectOrIndirectRemoteFile);
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
