//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationParametersWidget::SingleCellSimulationViewInformationParametersWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>())
{
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::retranslateUi()
{
    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::initialize(const QString &pFileName,
                                                                     CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    Q_UNUSED(pFileName);

    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Retrieve the property editor for the given file name or create one, if
    // none exists

    Core::PropertyEditorWidget *propertyEditor = mPropertyEditors.value(pFileName);

    if (!propertyEditor) {
        // No property editor exists for the given file name, so create one

        propertyEditor = new Core::PropertyEditorWidget(this);

        // Populate our property editor

        populateModel(propertyEditor, pCellmlFileRuntime);

        // Add our new property editor to ourselves

        addWidget(propertyEditor);

        // Keep track of our new property editor

        mPropertyEditors.insert(pFileName, propertyEditor);
    }

    // Set our retrieved property editor as our widget

    setCurrentWidget(propertyEditor);
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::finalize(const QString &pFileName)
{
    Q_UNUSED(pFileName);
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::cancelPropertyEditing()
{
    // Retrieve our current property editor, if any

    Core::PropertyEditorWidget *propertyEditor = qobject_cast<Core::PropertyEditorWidget *>(currentWidget());

    if (!propertyEditor)
        return;

    // Cancel the editing of our current property editor

    propertyEditor->cancelPropertyEditing();
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::populateModel(Core::PropertyEditorWidget *pPropertyEditor,
                                                                        CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime)
{
    Q_UNUSED(pPropertyEditor);
    Q_UNUSED(pCellmlFileRuntime);

//---GRY--- TO BE DONE...

    static int counter = 0;

    Core::Property *property = pPropertyEditor->addIntegerProperty();

    pPropertyEditor->setNonEditablePropertyItem(property->name(), "Dummy");
    pPropertyEditor->setIntegerPropertyItem(property->value(), ++counter);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
