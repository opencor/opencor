//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewinformationtraceswidget.h"
#include "singlecellsimulationviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include "ui_singlecellsimulationviewinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationWidget::SingleCellSimulationViewInformationWidget(QWidget *pParent) :
    QScrollArea(pParent),
    CommonWidget(pParent),
    mGui(new Ui::SingleCellSimulationViewInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Remove the frame around our scroll area

    setFrameShape(QFrame::NoFrame);

    // Create our collapsible widget

    mCollapsibleWidget = new Core::CollapsibleWidget(this);

    mCollapsibleWidget->setObjectName("Collapsible");

    // Create our simulation widget

    mSimulationWidget = new SingleCellSimulationViewInformationSimulationWidget(mCollapsibleWidget);

    mSimulationWidget->setObjectName("Simulation");

    // Create our solvers widget

    mSolversWidget = new SingleCellSimulationViewInformationSolversWidget(mCollapsibleWidget);

    mSolversWidget->setObjectName("Solvers");

    // Create our traces widget

//    mTracesWidget = new SingleCellSimulationViewInformationTracesWidget(mCollapsibleWidget);

//    mTracesWidget->setObjectName("Traces");
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...

    // Create our parameters widget

    mParametersWidget = new SingleCellSimulationViewInformationParametersWidget(mCollapsibleWidget);

    mParametersWidget->setObjectName("Parameters");

    // Add our simulation, solvers, traces and parameters widgets to our
    // collapsible widget

    mCollapsibleWidget->addWidget(mSimulationWidget);
    mCollapsibleWidget->addWidget(mSolversWidget);
//    mCollapsibleWidget->addWidget(mTracesWidget);
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...
    mCollapsibleWidget->addWidget(mParametersWidget);

    // Add our collapsible widget to our layout

    mGui->layout->addWidget(mCollapsibleWidget);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

SingleCellSimulationViewInformationWidget::~SingleCellSimulationViewInformationWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);

    // Retranslate the different titles of our collapsible widget

    mCollapsibleWidget->setHeaderTitle(0, tr("Simulation"));
    mCollapsibleWidget->setHeaderTitle(1, tr("Solvers"));
//    mCollapsibleWidget->setHeaderTitle(2, tr("Traces"));
//    mCollapsibleWidget->setHeaderTitle(3, tr("Parameters"));
mCollapsibleWidget->setHeaderTitle(2, tr("Parameters"));
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...

    // Retranslate our simulation, solvers, traces and parameters widgets

    mSimulationWidget->retranslateUi();
    mSolversWidget->retranslateUi();
//    mTracesWidget->retranslateUi();
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...
    mParametersWidget->retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our collapsible widget

    pSettings->beginGroup(mCollapsibleWidget->objectName());
        mCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our simulation widget

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our solvers widget

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our traces widget

//    pSettings->beginGroup(mTracesWidget->objectName());
//        mTracesWidget->loadSettings(pSettings);
//    pSettings->endGroup();
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...

    // Retrieve the settings of our parameters widget

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our collapsible widget

    pSettings->beginGroup(mCollapsibleWidget->objectName());
        mCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our simulation widget

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our solvers widget

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our traces widget

//    pSettings->beginGroup(mTracesWidget->objectName());
//        mTracesWidget->saveSettings(pSettings);
//    pSettings->endGroup();
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...

    // Keep track of the settings of our parameters widget

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

SingleCellSimulationViewInformationSimulationWidget * SingleCellSimulationViewInformationWidget::simulationWidget()
{
    // Return our simulation widget

    return mSimulationWidget;
}

//==============================================================================

SingleCellSimulationViewInformationSolversWidget * SingleCellSimulationViewInformationWidget::solversWidget()
{
    // Return our solvers widget

    return mSolversWidget;
}

//==============================================================================

//SingleCellSimulationViewInformationTracesWidget * SingleCellSimulationViewInformationWidget::tracesWidget()
//{
//    // Return our traces widget

//    return mTracesWidget;
//}
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...

//==============================================================================

SingleCellSimulationViewInformationParametersWidget * SingleCellSimulationViewInformationWidget::parametersWidget()
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::cancelEditing()
{
    // Cancel the editing of any of the information we support

    mSimulationWidget->cancelPropertyEditing();
    mSolversWidget->cancelPropertyEditing();
//    mTracesWidget->cancelPropertyEditing();
//---GRY--- THE ABOVE IS TEMPORARY, I.E. WE NEED TO BE ABLE TO PLOT DATA FOR A
//          DEMONSTRATION, BUT WON'T HAVE TO HANDLE TRACES IN TIME, SO...
    mParametersWidget->cancelPropertyEditing();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
