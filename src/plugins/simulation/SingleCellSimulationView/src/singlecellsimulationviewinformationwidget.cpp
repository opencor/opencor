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

    // Create our Solvers collapsible widget

    mSolversCollapsibleWidget = new Core::CollapsibleWidget(this);

    // Create our Traces collapsible widget

    mTracesCollapsibleWidget = new Core::CollapsibleWidget(this);

    // Create our Parameters collapsible widget

    mParametersCollapsibleWidget = new Core::CollapsibleWidget(this);

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
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::loadSettings(QSettings *pSettings)
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    //---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
