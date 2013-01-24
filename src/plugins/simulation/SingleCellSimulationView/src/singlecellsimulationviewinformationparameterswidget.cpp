//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "propertyeditorwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"

//==============================================================================

#include <QHeaderView>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewInformationParametersWidget::SingleCellSimulationViewInformationParametersWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>()),
    mColumnWidths(QList<int>())
{
    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::retranslateUi()
{
    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        mColumnWidths[i] = pSettings->value(SettingsColumnWidth.arg(i),
                                            mColumnWidths.at(i)).toInt();

}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i), mColumnWidths.at(i));
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

        // Initialise our property editor's columns' width

        for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
            propertyEditor->setColumnWidth(i, mColumnWidths.at(i));

        // Populate our property editor

        populateModel(propertyEditor, pCellmlFileRuntime);

        // Keep track of changes to columns' width

        connect(propertyEditor->header(), SIGNAL(sectionResized(int,int,int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

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
    // Populate our property editor with the model parameters

    Core::Property *section = 0;

    foreach (CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter, pCellmlFileRuntime->modelParameters()) {
        // Check whether the current model parameter is in the same component as
        // the previous one

        QString newComponent = modelParameter->component();

        if (!section || newComponent.compare(section->name()->text())) {
            // The current model parameter is in a different component, so
            // create a new section for the 'new' component

            section = pPropertyEditor->addSectionProperty();

            // Customise our section

            section->name()->setIcon(QIcon(":CellMLSupport_componentNode"));

            pPropertyEditor->setNonEditablePropertyItem(section->name(), newComponent);

            // Expand the section

            pPropertyEditor->setExpanded(section->name()->index(), true);
        }

        // Add the current model parameter to the 'current' component section

        Core::Property *property = pPropertyEditor->addDoubleProperty(section);

        property->name()->setIcon(QIcon(":CellMLSupport_variableNode"));

        pPropertyEditor->setNonEditablePropertyItem(property->name(), modelParameter->name());
        pPropertyEditor->setNonEditablePropertyItem(property->unit(), modelParameter->unit());
    }
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::propertyEditorSectionResized(const int &pLogicalIndex,
                                                                                       const int &pOldSize,
                                                                                       const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Update the column width of all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->header()->resizeSection(pLogicalIndex, pNewSize);

    // Keep track of the new column width

    mColumnWidths[pLogicalIndex] = pNewSize;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
