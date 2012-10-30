//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#include "singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include <QtDesigner/QDesignerComponents>
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerPropertyEditorInterface>

//==============================================================================

#include "ui_singlecellsimulationviewsimulationinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationInformationWidget::SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent) :
    Widget(defaultSize(0), pParent),
    mGui(new Ui::SingleCellSimulationViewSimulationInformationWidget),
    mFormEditor(QDesignerComponents::createFormEditor(0)),
    mPropertyEditor(QDesignerComponents::createPropertyEditor(mFormEditor, this)),
    mProperties(new QObject())

{
    // Set up the GUI

    mGui->setupUi(this);

    // Add our property editor to our layout

    mProperties->setProperty("Test", 3);

    mPropertyEditor->setObject(mProperties);

    mGui->layout->addWidget(mPropertyEditor);
}

//==============================================================================

SingleCellSimulationViewSimulationInformationWidget::~SingleCellSimulationViewSimulationInformationWidget()
{
    // Delete some internal objects

    delete mProperties;

    delete mPropertyEditor;
    delete mFormEditor;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewSimulationInformationWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);
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
