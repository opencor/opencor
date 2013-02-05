//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "cellmlfilevariable.h"
#include "propertyeditorwidget.h"
#include "singlecellsimulationviewinformationparameterswidget.h"
#include "singlecellsimulationviewsimulation.h"

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
    mModelParameters(QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeModelParameter *>()),
    mColumnWidths(QList<int>()),
    mSimulationData(0)
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
                                                                     CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                                                                     SingleCellSimulationViewSimulationData *pSimulationData)
{
    // Make sure that we have a CellML file runtime

    if (!pCellmlFileRuntime)
        return;

    // Keep track of the simulation data

    mSimulationData = pSimulationData;

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

        // Keep track of when some of the model's data has changed

        connect(pSimulationData, SIGNAL(dataChanged()),
                this, SLOT(updateProperties()));

        // Keep track of when the user changes a property value

        connect(propertyEditor, SIGNAL(propertyChanged(Core::Property *)),
                this, SLOT(propertyChanged(Core::Property *)));
connect(propertyEditor, SIGNAL(propertyChecked(Core::Property *, const bool &)),
        this, SIGNAL(propertyChecked(Core::Property *, const bool &)));
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...

        // Add our new property editor to ourselves

        addWidget(propertyEditor);

        // Keep track of our new property editor

        mPropertyEditors.insert(pFileName, propertyEditor);
    }

    // Set our retrieved property editor as our widget

    setCurrentWidget(propertyEditor);
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::updateProperties()
{
    // Retrieve our current property editor, if any

    Core::PropertyEditorWidget *propertyEditor = qobject_cast<Core::PropertyEditorWidget *>(currentWidget());

    if (!propertyEditor)
        return;

    // Update our property editor's data

    foreach (Core::Property *property, propertyEditor->properties()) {
        CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mModelParameters.value(property);

        if (modelParameter)
            switch (modelParameter->type()) {
            case CellMLSupport::CellmlFileRuntimeModelParameter::Constant:
            case CellMLSupport::CellmlFileRuntimeModelParameter::ComputedConstant:
                propertyEditor->setDoublePropertyItem(property->value(), mSimulationData->constants()[modelParameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::State:
                propertyEditor->setDoublePropertyItem(property->value(), mSimulationData->states()[modelParameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::Rate:
                propertyEditor->setDoublePropertyItem(property->value(), mSimulationData->rates()[modelParameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeModelParameter::Algebraic:
                propertyEditor->setDoublePropertyItem(property->value(), mSimulationData->algebraic()[modelParameter->index()]);

                break;
            default:
                // Either Voi or Undefined, so...

                ;
            }
    }
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::propertyChanged(Core::Property *pProperty)
{
    // Retrieve our current property editor, if any

    Core::PropertyEditorWidget *propertyEditor = qobject_cast<Core::PropertyEditorWidget *>(currentWidget());

    if (!propertyEditor)
        return;

    // Update our simulation data

    CellMLSupport::CellmlFileRuntimeModelParameter *modelParameter = mModelParameters.value(pProperty);

    if (modelParameter)
        switch (modelParameter->type()) {
        case CellMLSupport::CellmlFileRuntimeModelParameter::Constant:
            mSimulationData->constants()[modelParameter->index()] = propertyEditor->doublePropertyItem(pProperty->value());

            break;
        case CellMLSupport::CellmlFileRuntimeModelParameter::State:
            mSimulationData->states()[modelParameter->index()] = propertyEditor->doublePropertyItem(pProperty->value());

            break;
        default:
            // Either Voi, Rate, Algebraic or Undefined, so...

            ;
        }

    // Recompute our 'computed constants' and 'variables'

    mSimulationData->recomputeComputedConstantsAndVariables();
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

        QString crtComponent = modelParameter->component();

        if (!section || crtComponent.compare(section->name()->text())) {
            // The current model parameter is in a different component, so
            // create a new section for the 'new' component

            section = pPropertyEditor->addSectionProperty();

            section->name()->setIcon(QIcon(":CellMLSupport_componentNode"));

            pPropertyEditor->setStringPropertyItem(section->name(), crtComponent);
        }

        // Add the current model parameter to the 'current' component section
        // Note: in case of an algebraic variable, if its degree is equal to
        //       zero, then we are dealing with a 'proper' algebraic variable
        //       otherwise a rate variable. Now, there may be several rate
        //       variables with the same name (but different degrees) and a
        //       state variable with the same name will also exist. So, to
        //       distinguish between all of them, we 'customise' our variable's
        //       name by appending n single quotes with n the degree of our
        //       variable (that degree is zero for all but rate variables; e.g.
        //       for a state variable which name is V, the corresponding rate
        //       variables of degree 1, 2 and 3 will be V', V'' and V''',
        //       respectively)...

        bool canEditProperty =    (modelParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::Constant)
                               || (modelParameter->type() == CellMLSupport::CellmlFileRuntimeModelParameter::State);
        Core::Property *property = pPropertyEditor->addDoubleProperty(canEditProperty, true, section);

        property->name()->setIcon(QIcon(":CellMLSupport_variableNode"));

        pPropertyEditor->setStringPropertyItem(property->name(), modelParameter->name()+QString(modelParameter->degree(), '\''));
        pPropertyEditor->setStringPropertyItem(property->unit(), modelParameter->unit());

        // Keep track of the link between our property value and model parameter

        mModelParameters.insert(property, modelParameter);
    }

    // Expand all our properties

    pPropertyEditor->expandAll();
}

//==============================================================================

void SingleCellSimulationViewInformationParametersWidget::propertyEditorSectionResized(const int &pLogicalIndex,
                                                                                       const int &pOldSize,
                                                                                       const int &pNewSize)
{
    Q_UNUSED(pOldSize);

    // Prevent all our property editors from responding to an updating of their
    // columns' width

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        disconnect(propertyEditor->header(), SIGNAL(sectionResized(int,int,int)),
                   this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

    // Update the column width of all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->header()->resizeSection(pLogicalIndex, pNewSize);

    // Keep track of the new column width

    mColumnWidths[pLogicalIndex] = pNewSize;

    // Re-allow all our property editors to respond to an updating of their
    // columns' width

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        connect(propertyEditor->header(), SIGNAL(sectionResized(int,int,int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
