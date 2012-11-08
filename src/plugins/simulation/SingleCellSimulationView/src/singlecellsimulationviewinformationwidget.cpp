//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include "ui_singlecellsimulationviewinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationWidget::SingleCellSimulationViewInformationWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our Simulation collapsible widget

    mSimulationInformationWidget = new SingleCellSimulationViewSimulationInformationWidget(this);
    mSimulationCollapsibleWidget = new Core::CollapsibleWidget(QString(),
                                                               mSimulationInformationWidget,
                                                               this);

    mSimulationInformationWidget->setObjectName("SimulationInformation");
    mSimulationCollapsibleWidget->setObjectName("SimulationCollapsible");

mSimulationInformationWidget->setUnit("ms");
//---GRY--- THE ABOVE IS JUST FOR TESTING PURPOSES...

    // Create our Solvers collapsible widget

    mSolversCollapsibleWidget = new Core::CollapsibleWidget(this);

    mSolversCollapsibleWidget->setObjectName("SolversInformation");

    // Create our Traces collapsible widget

    mTracesCollapsibleWidget = new Core::CollapsibleWidget(this);

    mTracesCollapsibleWidget->setObjectName("TracesInformation");

    // Create our Parameters collapsible widget

    mParametersCollapsibleWidget = new Core::CollapsibleWidget(this);

    mParametersCollapsibleWidget->setObjectName("ParametersInformation");

    // Add our collapsible widgets to our layout
    // Note: we add a stretch at the end to make sure that our collapsible
    //       widgets take as little vertical space as possible...

    mGui->layout->addWidget(mSimulationCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mSolversCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mTracesCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mParametersCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addStretch(1);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

    // Select our first property

    mSimulationInformationWidget->selectFirstItem();

    // Make our simulation collapsible widget our focus proxy

    setFocusProxy(mSimulationCollapsibleWidget);
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

    // Retranslate the title of our collapsible widgets

    mSimulationCollapsibleWidget->setTitle(tr("Simulation"));
    mSolversCollapsibleWidget->setTitle(tr("Solvers"));
    mTracesCollapsibleWidget->setTitle(tr("Traces"));
    mParametersCollapsibleWidget->setTitle(tr("Parameters"));

    // Retranslate our various collapsible widgets' body

    mSimulationInformationWidget->retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our collapsible widgets and their bodies

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationInformationWidget->objectName());
        mSimulationInformationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our collapsible widgets and their bodies

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationInformationWidget->objectName());
        mSimulationInformationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
