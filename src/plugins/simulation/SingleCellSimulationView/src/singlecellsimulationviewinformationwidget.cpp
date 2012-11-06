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

    connect(mSimulationInformationWidget, SIGNAL(currentEditor(QWidget *)),
            this, SLOT(currentEditor(QWidget *)));

mSimulationInformationWidget->setUnit("ms");
//---GRY--- THE ABOVE IS JUST FOR TESTING PURPOSES...

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

    // Select our first property

    mSimulationInformationWidget->selectFirstNode();
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
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    //---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewInformationWidget::currentEditor(QWidget *pEditor)
{
    // The current editor has changed, meaning that either we are editing a
    // property or have stopped editing one, so update our focus proxy
    // accordingly

    if (pEditor) {
        // We are editing a property, so use its editor as our focus proxy and
        // make sure that it immediately gets the focus
        // Note: if we were not to immediately give the editor the focus, then
        //       the central widget would give the focus to our 'old' editor
        //       (see CentralWidget::updateGui()), so...

        setFocusProxy(pEditor);

        pEditor->setFocus();
    } else {
        // We have stopped editing a property, so reset our focus proxy and make
        // sure that we get the focus (see above for the reason)

        setFocusProxy(0);

        setFocus();
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
