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

#include "sedmlapidisablewarnings.h"
    #include "sedml/SedAlgorithm.h"
    #include "sedml/SedDocument.h"
    #include "sedml/SedOneStep.h"
    #include "sedml/SedUniformTimeCourse.h"
#include "sedmlapienablewarnings.h"

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

bool SingleCellViewWidget::isIndirectRemoteFile(const QString &pFileName) const
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
        return simulationWidget->save(pNewFileName);
    else
        return false;
}

//==============================================================================

void SingleCellViewWidget::fileOpened(const QString &pFileName)
{
    // Track the given file name

    mFileNames << pFileName;

    // Make sure that GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui();
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

        // Make sure that GUI of our simulation widgets is up to date

        foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
            simulationWidget->updateGui();
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
        mSimulationWidgets.insert(pNewFileName, simulationWidget);
        mSimulationWidgets.remove(pOldFileName);
    }

    // Let our simulation widgets know that the given file has been renamed

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->fileRenamed(pOldFileName, pNewFileName);

    // Make sure that GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui();
}

//==============================================================================

void SingleCellViewWidget::fileClosed(const QString &pFileName)
{
    // Stop tracking the given file name

    mFileNames.removeOne(pFileName);

    // Make sure that GUI of our simulation widgets is up to date

    foreach (SingleCellViewSimulationWidget *simulationWidget, mSimulationWidgets.values())
        simulationWidget->updateGui();
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
            currentSimulationWidget->updateSimulationResults(simulationWidget, simulationResultsSize, pForceUpdateSimulationResults);
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

bool SingleCellViewWidget::sedmlAlgorithmSupported(const libsedml::SedAlgorithm *pSedmlAlgorithm,
                                                   SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
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

    return true;
}

//==============================================================================

bool SingleCellViewWidget::sedmlFileSupported(SEDMLSupport::SedmlFile *pSedmlFile,
                                              SEDMLSupport::SedmlFileIssues &pSedmlFileIssues) const
{
//---ISSUE825--- TO BE DONE...
    // Make sure that there is only one model referenced in the given SED-ML
    // file and that it is of CellML type

    libsedml::SedDocument *sedmlDocument = pSedmlFile->sedmlDocument();

    if (sedmlDocument->getNumModels() == 1) {
        libsedml::SedModel *model = sedmlDocument->getModel(0);
        QString language = QString::fromStdString(model->getLanguage());

        if (   language.compare(SEDMLSupport::Language::Cellml)
            && language.compare(SEDMLSupport::Language::Cellml_1_0)
            && language.compare(SEDMLSupport::Language::Cellml_1_1)) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with a CellML file are supported"));

            return false;
        }
    } else {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one model are supported"));

        return false;
    }

    // Make sure that there is one or two simulations referenced in the given
    // SED-ML file

    int nbOfSimulations = sedmlDocument->getNumSimulations();

    if ((nbOfSimulations == 1) || (nbOfSimulations == 2)) {
        // Make sure that the first simulation is a uniform time course
        // simulation

        libsedml::SedSimulation *simulation = sedmlDocument->getSimulation(0);

        if (simulation->getTypeCode() != libsedml::SEDML_SIMULATION_UNIFORMTIMECOURSE) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with a uniform time course as a (first) simulation are supported"));

            return false;
        }

        // Make sure that the initial and output start time are the same, that
        // the output start and end times are different, and that the number of
        // points is greater than zero

        libsedml::SedUniformTimeCourse *uniformTimeCourse = static_cast<libsedml::SedUniformTimeCourse *>(simulation);
        double initialTime = uniformTimeCourse->getInitialTime();
        double outputStartTime = uniformTimeCourse->getOutputStartTime();
        double outputEndTime = uniformTimeCourse->getOutputEndTime();
        int nbOfPoints = uniformTimeCourse->getNumberOfPoints();

        if (initialTime != outputStartTime) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                             tr("only SED-ML files with the same initialTime and outputStartTime values are supported"));

            return false;
        }

        if (outputStartTime == outputEndTime) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("the outputStartTime and outputEndTime values must be different"));

            return false;
        }

        if (nbOfPoints <= 0) {
            pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                             tr("the numberOfPoints must be greater than zero"));

            return false;
        }

        // Make sure that the algorithm used for the first simulation is
        // supported

        if (!sedmlAlgorithmSupported(simulation->getAlgorithm(), pSedmlFileIssues))
            return false;

        // Check whether there is a second simulation

        simulation = sedmlDocument->getSimulation(1);

        if (simulation) {
            // There is a second simulation, so make sure that it is a one-step
            // simulation

            if (simulation->getTypeCode() != libsedml::SEDML_SIMULATION_ONESTEP) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                                 tr("only SED-ML files with a one-step as a second simulation are supported"));

                return false;
            }

            // Make sure that the step is greater than zero

            if (static_cast<libsedml::SedOneStep *>(simulation)->getStep() <= 0) {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("the step value must be greater than zero"));

                return false;
            }
        }
    } else {
        pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                         tr("only SED-ML files with one or two simulations are supported"));

        return false;
    }

    return true;
}

//==============================================================================

bool SingleCellViewWidget::combineArchiveSupported(COMBINESupport::CombineArchive *pCombineArchive,
                                                   COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const
{
//---ISSUE825--- TO BE DONE...
    // Load and check our COMBINE archive

    if (pCombineArchive->load() && pCombineArchive->isValid()) {
        // Make sure that there is only one master file in our COMBINE archive

        if (pCombineArchive->masterFiles().count() == 1) {
pCombineArchiveIssues << COMBINESupport::CombineArchiveIssue(COMBINESupport::CombineArchiveIssue::Information,
                                                             "still under development");

            return true;
        } else {
            pCombineArchiveIssues << COMBINESupport::CombineArchiveIssue(COMBINESupport::CombineArchiveIssue::Information,
                                                                         tr("only COMBINE archives with one master file are supported"));

            return false;
        }
    } else {
        pCombineArchiveIssues = pCombineArchive->issues();

        return false;
    }
}

//==============================================================================

void SingleCellViewWidget::retrieveCellmlFile(const QString &pFileName,
                                              CellMLSupport::CellmlFile *&pCellmlFile,
                                              SEDMLSupport::SedmlFile *pSedmlFile,
                                              SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                                              bool *pIsDirectOrIndirectRemoteFile) const
{
    // Make sure that we support our SED-ML

    if (!sedmlFileSupported(pSedmlFile, pSedmlFileIssues))
        return;

    // Retrieve the source of the CellML file, if any, referenced in our SED-ML
    // file

    QString modelSource = QString::fromStdString(pSedmlFile->sedmlDocument()->getModel(0)->getSource());

    // Check whether we are dealing with a local file (which is location is
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
        bool isLocalFile;
        QString dummy;

        Core::checkFileNameOrUrl(modelSource, isLocalFile, dummy);

        if (isLocalFile) {
            QString cellmlFileName = Core::nativeCanonicalFileName(QFileInfo(pFileName).path()+QDir::separator()+modelSource);

            if (QFile::exists(cellmlFileName)) {
                pCellmlFile = new CellMLSupport::CellmlFile(cellmlFileName);
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("%1 could not be found").arg(modelSource));
            }
        } else {
            QString fileContents;
            QString errorMessage;

            if (Core::readTextFromUrl(modelSource, fileContents, &errorMessage)) {
pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Information,
                                                 QString("%1 has been successfully read").arg(modelSource));
            } else {
                pSedmlFileIssues << SEDMLSupport::SedmlFileIssue(SEDMLSupport::SedmlFileIssue::Error,
                                                                 tr("%1 could not be opened (%2)").arg(modelSource, Core::formatMessage(errorMessage)));
            }
        }
    }
//---ISSUE825--- HANDLE THE CASE OF A REMOTE (CellML 1.1) FILE...
}

//==============================================================================

void SingleCellViewWidget::retrieveSedmlFile(COMBINESupport::CombineArchive *pCombineArchive,
                                             COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues) const
{
    // Make sure that we support our COMBINE archive

    if (!combineArchiveSupported(pCombineArchive, pCombineArchiveIssues))
        return;
//---ISSUE825--- TO BE DONE...
}

//==============================================================================

void SingleCellViewWidget::retrieveFileDetails(const QString &pFileName,
                                               CellMLSupport::CellmlFile *&pCellmlFile,
                                               SEDMLSupport::SedmlFile *&pSedmlFile,
                                               COMBINESupport::CombineArchive *&pCombineArchive,
                                               FileType &pFileType,
                                               SEDMLSupport::SedmlFileIssues &pSedmlFileIssues,
                                               COMBINESupport::CombineArchiveIssues &pCombineArchiveIssues,
                                               bool *pIsDirectOrIndirectRemoteFile) const
{
    // Determine the type of file we are dealing with

    pCellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
    pSedmlFile = pCellmlFile?0:SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName);
    pCombineArchive = pSedmlFile?0:COMBINESupport::CombineFileManager::instance()->combineArchive(pFileName);

    pFileType = pCellmlFile?CellmlFile:SedmlFile?SedmlFile:CombineArchive;

    pSedmlFileIssues = SEDMLSupport::SedmlFileIssues();
    pCombineArchiveIssues = COMBINESupport::CombineArchiveIssues();

    // In the case of a COMBINE archive, we need to retrieve the corresponding
    // SED-ML file while, in the case of a SED-ML file, we need to retrieve the
    // corresponding CellML file

    if (pCombineArchive)
        retrieveSedmlFile(pCombineArchive, pCombineArchiveIssues);

    if (pSedmlFile) {
        retrieveCellmlFile(pFileName, pCellmlFile, pSedmlFile, pSedmlFileIssues,
                           pIsDirectOrIndirectRemoteFile);
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
