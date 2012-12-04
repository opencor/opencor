//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"
#include "qtpropertybrowserwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationsolverswidget.h"
#include "singlecellsimulationviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

//---GRY---
#include <QtVariantPropertyManager>

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

    mSimulationWidget = new SingleCellSimulationViewInformationSimulationWidget(this);
    mSimulationCollapsibleWidget = new Core::CollapsibleWidget(QString(), mSimulationWidget, this);

    mSimulationWidget->setObjectName("Simulation");
    mSimulationCollapsibleWidget->setObjectName("SimulationCollapsible");

    // Create our Solvers collapsible widget

    mSolversWidget = new SingleCellSimulationViewInformationSolversWidget(this);
    mSolversCollapsibleWidget = new Core::CollapsibleWidget(QString(), mSolversWidget, this);

    mSolversWidget->setObjectName("Solvers");
    mSolversCollapsibleWidget->setObjectName("SolversCollapsible");

    // Create our Traces collapsible widget

    mTracesCollapsibleWidget = new Core::CollapsibleWidget(this);

    mTracesCollapsibleWidget->setObjectName("TracesCollapsible");

    // Create our Parameters collapsible widget

    mParametersWidget = new SingleCellSimulationViewInformationParametersWidget(this);
    mParametersCollapsibleWidget = new Core::CollapsibleWidget(QString(), mParametersWidget, this);

    mParametersWidget->setObjectName("Parameters");
    mParametersCollapsibleWidget->setObjectName("ParametersCollapsible");

    // Add our collapsible widgets to our layout

    mGui->layout->addWidget(mSimulationCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mSolversCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mTracesCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));
    mGui->layout->addWidget(mParametersCollapsibleWidget);
    mGui->layout->addWidget(Core::newLineWidget(this));

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();

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

    mSimulationWidget->retranslateUi();
    mSolversWidget->retranslateUi();
    mParametersWidget->retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our simulation collapsible widget and its body

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our solvers collapsible widget and its body

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our traces collapsible widget and its body

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our parameters collapsible widget and its body

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our simulation collapsible widget and its body

    pSettings->beginGroup(mSimulationCollapsibleWidget->objectName());
        mSimulationCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our solvers collapsible widget and its body

    pSettings->beginGroup(mSolversCollapsibleWidget->objectName());
        mSolversCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our traces collapsible widget and its body

    pSettings->beginGroup(mTracesCollapsibleWidget->objectName());
        mTracesCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our parameters collapsible widget and its body

    pSettings->beginGroup(mParametersCollapsibleWidget->objectName());
        mParametersCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

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

SingleCellSimulationViewInformationParametersWidget * SingleCellSimulationViewInformationWidget::parametersWidget()
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
