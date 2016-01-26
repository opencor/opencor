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

/*---GRY---
#include "cellmlfilemanager.h"
#include "cellmlfileruntime.h"
#include "combinearchive.h"
#include "combinefilemanager.h"
#include "combinesupportplugin.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "datastoreinterface.h"
#include "filemanager.h"
#include "progressbarwidget.h"
#include "propertyeditorwidget.h"
#include "sedmlfile.h"
#include "sedmlfilemanager.h"
#include "sedmlsupportplugin.h"
*/
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
/*---GRY---
    // Keep track of our simulation data for our previous model and finalise a
    // few things, if needed

    SingleCellViewInformationWidget *informationWidget = mContentsWidget->informationWidget();
    SingleCellViewGraphPanelsWidget *graphPanelsWidget = mContentsWidget->graphPanelsWidget();

    SingleCellViewInformationSimulationWidget *simulationWidget = informationWidget->simulationWidget();
    SingleCellViewInformationSolversWidget *solversWidget = informationWidget->solversWidget();
    SingleCellViewInformationGraphsWidget *graphsWidget = informationWidget->graphsWidget();

    SingleCellViewSimulation *previousSimulation = mSimulation;

    if (previousSimulation) {
        // There is a previous simulation, so backup our simulation, solvers and
        // graph panels' settings

        QString prevFileName = previousSimulation->fileName();

        simulationWidget->backup(prevFileName);
        solversWidget->backup(prevFileName);
        graphsWidget->backup(prevFileName);

        graphPanelsWidget->backup(prevFileName);

        // Keep track of the status of the reset action (if needed), the
        // simulation delay and the development mode

        if (!mGui->actionDevelopmentMode->isChecked())
            mResets.insert(prevFileName, mGui->actionResetModelParameters->isEnabled());

        mDelays.insert(prevFileName, mDelayWidget->value());
        mDevelopmentModes.insert(prevFileName, mGui->actionDevelopmentMode->isChecked());
    }

    // Stop keeping track of certain things (so that updatePlot() doesn't get
    // called unnecessarily)
    // Note: see the corresponding code at the end of this method...

    disconnect(simulationWidget, SIGNAL(propertyChanged(Core::Property *)),
               this, SLOT(simulationPropertyChanged(Core::Property *)));

    // Determine the type of our file

    CellMLSupport::CellmlFileManager *cellmlFileManager = CellMLSupport::CellmlFileManager::instance();

    mFileType = mFileTypes.value(pFileName);

    if (mFileType == Unknown) {
        // No file type exists for the file, so determine it

        if (cellmlFileManager->cellmlFile(pFileName))
            mFileType = CellmlFile;
        else if (SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName))
            mFileType = SedmlFile;
        else
            mFileType = CombineArchive;

        // Keep track of our file type

        mFileTypes.insert(pFileName, mFileType);
    }

    // Check whether we are to deal with a CellML or a SED-ML file

    QString sedmlFileName = QString();
    QString combineIssue = QString();

    if (mFileType == SedmlFile) {
        sedmlFileName = pFileName;
    } else if (mFileType == CombineArchive) {
        // We are dealing with a COMBINE archive, so its master file should be
        // the SED-ML file we are after

        COMBINESupport::CombineArchive *combineArchive = COMBINESupport::CombineFileManager::instance()->combineArchive(pFileName);
        COMBINESupport::CombineArchiveFiles masterFiles = combineArchive->masterFiles();

        foreach (const COMBINESupport::CombineArchiveFile &masterFile, masterFiles) {
            if (masterFile.format() == COMBINESupport::CombineArchiveFile::Sedml) {
                if (sedmlFileName.isEmpty()) {
                    sedmlFileName = combineArchive->location(masterFile);
                } else {
                    sedmlFileName = QString();

                    combineIssue = tr("more than one master SED-ML file was found");

                    break;
                }
            }
        }

        if (sedmlFileName.isEmpty() && combineIssue.isEmpty())
            combineIssue = tr("no master SED-ML file could be found");
    }

    // Retrieve our simulation object for the current model, if any

    bool newSimulation = false;
    CellMLSupport::CellmlFile *cellmlFile = (mFileType == CellmlFile)?cellmlFileManager->cellmlFile(pFileName):0;
    CellMLSupport::CellmlFileRuntime *cellmlFileRuntime = (mFileType == CellmlFile)?cellmlFile->runtime():0;

    mSimulation = mSimulations.value(pFileName);

    if (!mSimulation) {
        // No simulation object currently exists for the model, so create one

        mSimulation = new SingleCellViewSimulation(pFileName, cellmlFileRuntime, mSolverInterfaces);

        newSimulation = true;

        // Initialise our simulation object's delay

        updateDelayValue(mDelayWidget->value());

        // Create a few connections

        connect(mSimulation, SIGNAL(running(const bool &)),
                this, SLOT(simulationRunning(const bool &)));
        connect(mSimulation, SIGNAL(paused()),
                this, SLOT(simulationPaused()));
        connect(mSimulation, SIGNAL(stopped(const qint64 &)),
                this, SLOT(simulationStopped(const qint64 &)));

        connect(mSimulation, SIGNAL(error(const QString &)),
                this, SLOT(simulationError(const QString &)));

        connect(mSimulation->data(), SIGNAL(modified(const bool &)),
                this, SLOT(simulationDataModified(const bool &)));

        // Keep track of our simulation object

        mSimulations.insert(pFileName, mSimulation);
    }

    // Retrieve the status of the reset action, the simulation delay and the
    // development mode

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    if (!mDevelopmentModes.value(pFileName))
        mGui->actionResetModelParameters->setEnabled(mResets.value(pFileName));

    mGui->actionDevelopmentMode->setEnabled(fileManagerInstance->isReadableAndWritable(pFileName));

    mDelayWidget->setValue(mDelays.value(pFileName));
    mGui->actionDevelopmentMode->setChecked(mDevelopmentModes.value(pFileName));

    // Reset our file tab icon and update our progress bar
    // Note: they may not both be necessary, but we never know...

    resetFileTabIcon(mSimulation);

    mProgressBarWidget->setValue(mOldSimulationResultsSizes.value(mSimulation)/mSimulation->size());

    // Retrieve our variable of integration, if possible

    bool validCellmlFileRuntime = cellmlFileRuntime && cellmlFileRuntime->isValid();

    CellMLSupport::CellmlFileRuntimeParameter *variableOfIntegration = validCellmlFileRuntime?cellmlFileRuntime->variableOfIntegration():0;

    // Output some information about our CellML file

    QString information = QString();

    if (!mOutputWidget->document()->isEmpty())
        information += "<hr/>\n";

    QString fileName = fileManagerInstance->isNew(pFileName)?
                           tr("File")+" #"+QString::number(fileManagerInstance->newIndex(pFileName)):
                           fileManagerInstance->isRemote(pFileName)?
                               fileManagerInstance->url(pFileName):
                               pFileName;

    information += "<strong>"+fileName+"</strong>"+OutputBrLn;
    information += OutputTab+"<strong>"+tr("Runtime:")+"</strong> ";

    if (variableOfIntegration) {
        // A variable of integration could be retrieved for our CellML file, so
        // we can also output the model type

        QString additionalInformation = QString();

        if (cellmlFileRuntime->needNlaSolver())
            additionalInformation = " + "+tr("NLA system(s)");

        information += "<span"+OutputGood+">"+tr("valid")+"</span>."+OutputBrLn;
        information += QString(OutputTab+"<strong>"+tr("Model type:")+"</strong> <span"+OutputInfo+">%1%2</span>."+OutputBrLn).arg((cellmlFileRuntime->modelType() == CellMLSupport::CellmlFileRuntime::Ode)?tr("ODE"):tr("DAE"),
                                                                                                                                   additionalInformation);
    } else {
        // We couldn't retrieve a variable a variable of integration, which
        // means that we either don't have a runtime or we have one, but it's
        // not valid or it's valid but we really don't have a variable of
        // integration
        // Note: in the case of a valid runtime and no variable of integration,
        //       we really shouldn't consider the runtime to be valid, hence we
        //       handle this case here...

        mErrorType = InvalidCellmlFile;

        updateInvalidModelMessageWidget();

        information += "<span"+OutputBad+">"+(cellmlFileRuntime?tr("invalid"):tr("none"))+"</span>."+OutputBrLn;

        if (validCellmlFileRuntime) {
            // We have a valid runtime, but no variable of integration, which
            // means that the model doesn't contain any ODE or DAE

            information += OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+tr("the model must have at least one ODE or DAE")+".</span>"+OutputBrLn;
        } else if (mFileType == CellmlFile) {
            // We don't have a valid runtime, so either there are some problems
            // with the CellML file or its runtime

            foreach (const CellMLSupport::CellmlFileIssue &issue,
                     cellmlFileRuntime?cellmlFileRuntime->issues():cellmlFile->issues()) {
                information += QString(OutputTab+"<span"+OutputBad+"><strong>%1</strong> %2.</span>"+OutputBrLn).arg((issue.type() == CellMLSupport::CellmlFileIssue::Error)?tr("Error:"):tr("Warning:"),
                                                                                                                     issue.message());
            }
        } else if ((mFileType == CombineArchive) && !combineIssue.isEmpty()) {
            information += QString(OutputTab+"<span"+OutputBad+"><strong>"+tr("Error:")+"</strong> "+combineIssue+".</span>");
        }
    }

    output(information);

    // Enable/disable our run/pause action depending on whether we have a
    // variable of integration

    mGui->actionRunPauseResumeSimulation->setEnabled(variableOfIntegration);

    // Update our simulation mode

    updateSimulationMode();

    // Initialise our contents widget and make sure that we have the required
    // type(s) of solvers

    bool validSimulationEnvironment = false;

    if (variableOfIntegration) {
        // Show our contents widget in case it got previously hidden
        // Note: indeed, if it was to remain hidden then some initialisations
        //       wouldn't work (e.g. the solvers widget has a property editor,
        //       which all properties need to be removed and if the contents
        //       widget is not visible, then upon repopulating the property
        //       editor, scrollbars will be shown even though they are not
        //       needed)...

        mContentsWidget->setVisible(true);

        // Check whether we have at least one ODE or DAE solver and, if needed,
        // at least one NLA solver

        if (cellmlFileRuntime->needNlaSolver()) {
            if (solversWidget->nlaSolvers().isEmpty()) {
                if (cellmlFileRuntime->needOdeSolver()) {
                    if (solversWidget->odeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    } else {
                        simulationError(tr("the model needs both an ODE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                    }
                } else {
                    if (solversWidget->daeSolvers().isEmpty()) {
                        simulationError(tr("the model needs both a DAE and an NLA solver, but none are available"),
                                        InvalidSimulationEnvironment);
                    } else {
                        simulationError(tr("the model needs both a DAE and an NLA solver, but no NLA solver is available"),
                                        InvalidSimulationEnvironment);
                    }
                }
            } else if (   cellmlFileRuntime->needOdeSolver()
                       && solversWidget->odeSolvers().isEmpty()) {
                simulationError(tr("the model needs both an ODE and an NLA solver, but no ODE solver is available"),
                                InvalidSimulationEnvironment);
            } else if (   cellmlFileRuntime->needDaeSolver()
                       && solversWidget->daeSolvers().isEmpty()) {
                    simulationError(tr("the model needs both a DAE and an NLA solver, but no DAE solver is available"),
                                    InvalidSimulationEnvironment);
            } else {
                validSimulationEnvironment = true;
            }
        } else if (   cellmlFileRuntime->needOdeSolver()
                   && solversWidget->odeSolvers().isEmpty()) {
            simulationError(tr("the model needs an ODE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else if (   cellmlFileRuntime->needDaeSolver()
                   && solversWidget->daeSolvers().isEmpty()) {
            simulationError(tr("the model needs a DAE solver, but none is available"),
                            InvalidSimulationEnvironment);
        } else {
            validSimulationEnvironment = true;
        }
    }

    // Show/hide some widgets depending on whether we have a valid simulation
    // environment

    bool prevValidSimulationEnvironment = mInvalidModelMessageWidget->isHidden();

    mToolBarWidget->setVisible(validSimulationEnvironment);
    mTopSeparator->setVisible(validSimulationEnvironment);

    mContentsWidget->setVisible(validSimulationEnvironment);
    mInvalidModelMessageWidget->setVisible(!validSimulationEnvironment);

    mBottomSeparator->setVisible(validSimulationEnvironment);
    mProgressBarWidget->setVisible(validSimulationEnvironment);

    // Make sure that the last output message is visible
    // Note: indeed, to (re)show some widgets (see above) might change the
    //       height of our output widget, messing up the vertical scroll bar a
    //       bit (if visible), resulting in the output being shifted a bit...

    if (prevValidSimulationEnvironment != validSimulationEnvironment) {
        QCoreApplication::processEvents();

        mOutputWidget->ensureCursorVisible();
    }

    // Some additional initialisations in case we have a valid simulation
    // environment

    if (validSimulationEnvironment) {
        // Initialise our GUI's simulation, solvers, graphs and graph panels
        // widgets, but not parameters widget
        // Note #1: this will also initialise some of our simulation data (i.e.
        //          our simulation's starting point and simulation's NLA
        //          solver's properties), which is needed since we want to be
        //          able to reset our simulation below...
        // Note #2: to initialise our graphs widget will result in some graphs
        //          being shown/hidden and, therefore, in graphsUpdated() being
        //          called. Yet, we don't want graphsUpdated() to update our
        //          plots. Indeed, if it did, then all of our plots' axes'
        //          values would be reset while we want to keep the ones we just
        //          retrieved (thus making it possible for the user to have
        //          different views for different files). So, for this to work
        //          we use mCanUpdatePlotsForUpdatedGraphs, and then 'manually'
        //          replot our plots, once we know which graphs are to be
        //          shown/hidden. We could do the initialisation before the
        //          setting of the plots' axes' values, but then we could see
        //          the graphs being plotted twice. Once after the plots' axes'
        //          values have been reset following the call to graphsUpdated()
        //          and another after we update our plots' axes' values. This is
        //          clearly not neat, hence the current solution...
        // Note #3: to initialise our parameters widget now would result in some
        //          default (in the computer science sense, i.e. wrong) values
        //          being visible for a split second before they get properly
        //          updated. So, instead, we initialise whatever needs
        //          initialising (e.g. NLA solver) so that we can safely compute
        //          our model parameters before showing their values...

        simulationWidget->initialize(pFileName, cellmlFileRuntime, mSimulation);
        solversWidget->initialize(pFileName, cellmlFileRuntime, mSimulation);

        mCanUpdatePlotsForUpdatedGraphs = false;
            graphsWidget->initialize(pFileName, cellmlFileRuntime, mSimulation);
        mCanUpdatePlotsForUpdatedGraphs = true;

        graphPanelsWidget->initialize(pFileName);

        // Reset both the simulation's data and results (well, initialise in the
        // case of its data), in case we are dealing with a new simulation

        if (newSimulation) {
            mSimulation->data()->reset();
            mSimulation->results()->reset(false);
        }

        // Retrieve our simulation and solvers properties since they may have
        // an effect on our parameter values (as well as result in some solver
        // properties being shown/hidden)

        if (newSimulation || pReloadingView) {
            updateSimulationProperties();
            updateSolversProperties();
        }

        // Now, we can safely update our parameters widget since our model
        // parameters have been computed

        informationWidget->parametersWidget()->initialize(pFileName, cellmlFileRuntime, mSimulation, pReloadingView);

        // Update our plots since our 'new' simulation properties may have
        // affected them

        if (newSimulation || pReloadingView) {
            foreach (SingleCellViewGraphPanelPlotWidget *plot, mPlots)
                updatePlot(plot);
        }
    }

    // Resume the tracking of certain things
    // Note: see the corresponding code at the beginning of this method...

    connect(simulationWidget, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(simulationPropertyChanged(Core::Property *)));
*/
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

/*---GRY---
        mFileTypes.remove(pFileName);
        mSimulations.remove(pFileName);
*/
    SingleCellViewSimulation *simulation = simulationWidget->simulation();
    qulonglong simulationResultsSize = simulation->results()->size();

    if (   pForceUpdateSimulationResults
        || (simulationResultsSize != mSimulationResultsSizes.value(pFileName))) {
        mSimulationResultsSizes.insert(pFileName, simulationResultsSize);

/*---GRY---
        if (simulation == mSimulation) {
            mFileType = Unknown;
            mSimulation = 0;
        }
*/
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
/*---GRY---
    // Replace the old file name with the new one in our various trackers

    if (mFileTypes.contains(pOldFileName)) {
        mFileTypes.insert(pNewFileName, mFileTypes.value(pOldFileName));
        mFileTypes.remove(pOldFileName);
    }

    SingleCellViewSimulation *simulation = mSimulations.value(pOldFileName);

    if (simulation) {
        simulation->setFileName(pNewFileName);

        mSimulations.insert(pNewFileName, simulation);
        mSimulations.remove(pOldFileName);
    }

    if (mProgresses.contains(pOldFileName)) {
        mProgresses.insert(pNewFileName, mProgresses.value(pOldFileName));
        mProgresses.remove(pOldFileName);
    }

    if (mResets.contains(pOldFileName)) {
        mResets.insert(pNewFileName, mResets.value(pOldFileName));
        mResets.remove(pOldFileName);
    }

    if (mDelays.contains(pOldFileName)) {
        mDelays.insert(pNewFileName, mDelays.value(pOldFileName));
        mDelays.remove(pOldFileName);
    }

    if (mDevelopmentModes.contains(pOldFileName)) {
        mDevelopmentModes.insert(pNewFileName, mDevelopmentModes.value(pOldFileName));
        mDevelopmentModes.remove(pOldFileName);
    }

    if (mLockedDevelopmentModes.contains(pOldFileName)) {
        mLockedDevelopmentModes.insert(pNewFileName, mLockedDevelopmentModes.value(pOldFileName));
        mLockedDevelopmentModes.remove(pOldFileName);
    }

    // Let our graphs widget know that the given file has been renamed
*/
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
