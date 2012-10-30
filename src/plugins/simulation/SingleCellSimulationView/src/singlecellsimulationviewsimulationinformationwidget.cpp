//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#include "coreutils.h"
#include "singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

#include <QLabel>
#include <QLineEdit>
#include <QSettings>

//==============================================================================

#include "ui_singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationInformationWidget::SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent) :
    Widget(defaultSize(0), pParent),
    mGui(new Ui::SingleCellSimulationViewSimulationInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our Start Point row

    mStartingPointLabel = Core::newLabel(QString(), 1.0, true, this);
    mStartingPointField = new QLineEdit(this);

    mGui->layout->addRow(mStartingPointLabel, mStartingPointField);

    // Some further initialisations which are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

SingleCellSimulationViewSimulationInformationWidget::~SingleCellSimulationViewSimulationInformationWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);

    // Retranslate the label of our rows

    mStartingPointLabel->setText(tr("Starting point:"));
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::loadSettings(QSettings *pSettings)
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::saveSettings(QSettings *pSettings) const
{
    //---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
