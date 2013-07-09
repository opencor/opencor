//==============================================================================
// Single cell view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewsimulation.h"

//==============================================================================

#include <QHeaderView>
#include <QMenu>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationParametersWidget::SingleCellViewInformationParametersWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>()),
    mContextMenus(QMap<Core::PropertyEditorWidget *, QMenu *>()),
    mParameters(QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mColumnWidths(QList<int>()),
    mPropertyEditor(0),
    mSimulationData(0)
{
    // Determine the default width of each column of our property editors

    Core::PropertyEditorWidget *tempPropertyEditor = new Core::PropertyEditorWidget(this);

    for (int i = 0, iMax = tempPropertyEditor->header()->count(); i < iMax; ++i)
        mColumnWidths.append(tempPropertyEditor->columnWidth(i));

    delete tempPropertyEditor;
}

//==============================================================================

void SingleCellViewInformationParametersWidget::retranslateContextMenu(QMenu *pContextMenu)
{
    // Retranslate our context menu

    pContextMenu->actions().at(0)->setText(tr("Plot Against Variable of Integration"));
    pContextMenu->actions().at(1)->setText(tr("Plot Against"));
}

//==============================================================================

void SingleCellViewInformationParametersWidget::retranslateUi()
{
    // Retranslate all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->retranslateUi();

    // Retranslate all our context menus

    foreach (QMenu *contextMenu, mContextMenus)
        retranslateContextMenu(contextMenu);

    // Retranslate the tool tip of all our parameters

    updateToolTips();
}

//==============================================================================

static const QString SettingsColumnWidth = "ColumnWidth%1";

//==============================================================================

void SingleCellViewInformationParametersWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        mColumnWidths[i] = pSettings->value(SettingsColumnWidth.arg(i),
                                            mColumnWidths.at(i)).toInt();

}

//==============================================================================

void SingleCellViewInformationParametersWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
        pSettings->setValue(SettingsColumnWidth.arg(i), mColumnWidths.at(i));
}

//==============================================================================

void SingleCellViewInformationParametersWidget::initialize(const QString &pFileName,
                                                           CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewSimulationData *pSimulationData)
{
    // Keep track of the simulation data

    mSimulationData = pSimulationData;

    // Retrieve the property editor for the given file name or create one, if
    // none exists

    mPropertyEditor = mPropertyEditors.value(pFileName);

    if (!mPropertyEditor) {
        // No property editor exists for the given file name, so create one

        mPropertyEditor = new Core::PropertyEditorWidget(this);

        // Initialise our property editor's columns' width

        for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
            mPropertyEditor->setColumnWidth(i, mColumnWidths.at(i));

        // Populate our property editor

        populateModel(pRuntime);

        // Create and populate our property editor's context menu

        QMenu *contextMenu = new QMenu(this);

        populateContextMenu(contextMenu, pRuntime);

        // We want our own context menu for our property editor

        mPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(mPropertyEditor, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(propertyEditorContextMenu(const QPoint &)));

        // Keep track of changes to columns' width

        connect(mPropertyEditor->header(), SIGNAL(sectionResized(int,int,int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

        // Keep track of when some of the model's data has changed

        connect(pSimulationData, SIGNAL(updated(const double &)),
                this, SLOT(updateParameters(const double &)));

        // Keep track of when the user changes a property value

        connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
                this, SLOT(propertyChanged(Core::Property *)));

        // Add our new property editor to ourselves

        addWidget(mPropertyEditor);

        // Keep track of our new property editor and its context menu

        mPropertyEditors.insert(pFileName, mPropertyEditor);
        mContextMenus.insert(mPropertyEditor, contextMenu);
    }

    // Set our retrieved property editor as our current widget

    setCurrentWidget(mPropertyEditor);

    // Update the tool tip of all our parameters
    // Note: this is in case the user changed the locale and then switched to a
    //       different file...

    updateToolTips();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::finalize(const QString &pFileName)
{
    // Remove any track of our property editor and its context menu

    mContextMenus.remove(mPropertyEditors.value(pFileName));
    mPropertyEditors.remove(pFileName);
}

//==============================================================================

void SingleCellViewInformationParametersWidget::updateParameters(const double &pCurrentPoint)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Update our property editor's data

    foreach (Core::Property *property, mPropertyEditor->properties()) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(property);

        if (parameter)
            switch (parameter->type()) {
            case CellMLSupport::CellmlFileRuntimeParameter::Voi:
                mPropertyEditor->setDoublePropertyItem(property->value(), pCurrentPoint);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
                mPropertyEditor->setDoublePropertyItem(property->value(), mSimulationData->constants()[parameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Rate:
                mPropertyEditor->setDoublePropertyItem(property->value(), mSimulationData->rates()[parameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::State:
                mPropertyEditor->setDoublePropertyItem(property->value(), mSimulationData->states()[parameter->index()]);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
                mPropertyEditor->setDoublePropertyItem(property->value(), mSimulationData->algebraic()[parameter->index()]);

                break;
            default:
                // Either Voi or Undefined, so...

                ;
            }
    }

    // Check whether any of our properties has actually been modified

    mSimulationData->checkForModifications();

    // Update the tool tip of all our parameters

    updateToolTips();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyChanged(Core::Property *pProperty)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Update our simulation data

    CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(pProperty);

    if (parameter)
        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            mSimulationData->constants()[parameter->index()] = mPropertyEditor->doublePropertyItem(pProperty->value());

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            mSimulationData->states()[parameter->index()] = mPropertyEditor->doublePropertyItem(pProperty->value());

            break;
        default:
            // Either Voi, ComputedConstant, Rate, Algebraic or Undefined, so...

            ;
        }

    // Recompute our 'computed constants' and 'variables'
    // Note: we would normally call mSimulationData->checkForModifications()
    //       after recomputing our 'computed constants' and 'variables, but the
    //       recomputation will eventually result in updateParameters() abvove
    //       to be called and it will check for modifications, so...

    mSimulationData->recomputeComputedConstantsAndVariables();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::finishPropertyEditing()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Finish the editing of our current property editor

    mPropertyEditor->finishPropertyEditing();
}

//==============================================================================

QIcon SingleCellViewInformationParametersWidget::parameterIcon(const CellMLSupport::CellmlFileRuntimeParameter::ParameterType &pParameterType)
{
    // Return an icon that illustrates the type of a parameter

    switch (pParameterType) {
    case CellMLSupport::CellmlFileRuntimeParameter::Voi:
        return QIcon(":SingleCellView_voi");
    case CellMLSupport::CellmlFileRuntimeParameter::Constant:
        return QIcon(":SingleCellView_constant");
    case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
        return QIcon(":SingleCellView_computedConstant");
    case CellMLSupport::CellmlFileRuntimeParameter::Rate:
        return QIcon(":SingleCellView_rate");
    case CellMLSupport::CellmlFileRuntimeParameter::State:
        return QIcon(":SingleCellView_state");
    case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
        return QIcon(":SingleCellView_algebraic");
    default:
        // We are dealing with a type of parameter which is of no interest to us
        // Note: we should never reach this point...

        return QIcon(":CellMLSupport_errorNode");
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateModel(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid any flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Populate our property editor with the parameters

    Core::Property *sectionProperty = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component as the
        // previous one

        QString currentComponent = parameter->component();

        if (   !sectionProperty
            ||  currentComponent.compare(sectionProperty->name()->text())) {
            // The current parameter is in a different component, so create a
            // new section for the 'new' component

            sectionProperty = mPropertyEditor->addSectionProperty();

            mPropertyEditor->setStringPropertyItem(sectionProperty->name(), currentComponent);
        }

        // Add the current parameter to the 'current' component section
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

        bool parameterEditable =    (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)
                                 || (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State);

        Core::Property *property = mPropertyEditor->addDoubleProperty(QString(), parameterEditable, sectionProperty);

        property->name()->setIcon(parameterIcon(parameter->type()));

        mPropertyEditor->setStringPropertyItem(property->name(), parameter->formattedName());
        mPropertyEditor->setStringPropertyItem(property->unit(), parameter->formattedUnit(pRuntime->variableOfIntegration()->unit()));

        // Keep track of the link between our property value and parameter

        mParameters.insert(property, parameter);
    }

    // Update the tool tip of all our parameters

    updateToolTips();

    // Expand all our properties

    mPropertyEditor->expandAll();

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateContextMenu(QMenu *pContextMenu,
                                                                    CellMLSupport::CellmlFileRuntime *pRuntime)
{
    Q_UNUSED(pRuntime);

    // Create our two main menu items

    QAction *voiAction = pContextMenu->addAction(QString());
    QMenu *plotAgainstMenu = new QMenu(pContextMenu);

    pContextMenu->addAction(plotAgainstMenu->menuAction());

    // Initialise our two main menu items

    retranslateContextMenu(pContextMenu);

    // Create a connection to handle the plotting requirement against our
    // variable of integration

    connect(voiAction, SIGNAL(triggered()),
            this, SLOT(emitGraphRequired()));

    // Keep track of the parameter associated with our first main menu item

    mParameterActions.insert(voiAction, pRuntime->variableOfIntegration());

    // Populate our property editor with the parameters

    QMenu *componentMenu = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component as the
        // previous one

        QString currentComponent = parameter->component();

        if (   !componentMenu
            ||  currentComponent.compare(componentMenu->menuAction()->text())) {
            // The current parameter is in a different component, so create a
            // new menu for the 'new' component

            componentMenu = new QMenu(currentComponent, pContextMenu);

            plotAgainstMenu->addMenu(componentMenu);
        }

        // Add the current parameter to the 'current' component menu
        // Note: in case of an algebraic variable, see the corresponding note in
        //       populateModel() above...

        QAction *parameterAction = componentMenu->addAction(parameterIcon(parameter->type()),
                                                            parameter->name()+QString(parameter->degree(), '\''));

        // Create a connection to handle the plotting requirement against our
        // parameter

        connect(parameterAction, SIGNAL(triggered()),
                this, SLOT(emitGraphRequired()));

        // Keep track of the parameter associated with our model parameter
        // action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::updateToolTips()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Update the tool tip of all our property editor's properties

    foreach (Core::Property *property, mPropertyEditor->properties()) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(property);

        if (parameter) {
            QString parameterType = QString();

            switch (parameter->type()) {
            case CellMLSupport::CellmlFileRuntimeParameter::Voi:
                parameterType = tr("variable of integration");

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Constant:
                parameterType = tr("constant");

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
                parameterType = tr("computed constant");

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Rate:
                parameterType = tr("rate");

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::State:
                parameterType = tr("state");

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
                parameterType = tr("algebraic");

                break;
            default:
                // We are dealing with a type of parameter which is of no
                // interest to us, so do nothing...
                // Note: we should never reach this point...

                ;
            }

            QString parameterToolTip = property->name()->text()+tr(": ")+property->value()->text()+" "+property->unit()->text()+" ("+parameterType+")";

            property->name()->setToolTip(parameterToolTip);
            property->value()->setToolTip(parameterToolTip);
            property->unit()->setToolTip(parameterToolTip);
        }
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyEditorContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Create a custom context menu for our property editor, based on what is
    // underneath our mouse pointer

    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Retrieve our current property, if any

    Core::Property *currentProperty = mPropertyEditor->currentProperty();

    if (!currentProperty)
        return;

    // Make sure that our current property is not a section

    if (currentProperty->name()->type() == Core::PropertyItem::Section)
        return;

    // Generate and show the context menu

    mContextMenus.value(mPropertyEditor)->exec(QCursor::pos());
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyEditorSectionResized(const int &pLogicalIndex,
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

void SingleCellViewInformationParametersWidget::emitGraphRequired()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Let people know that we want to plot the current parameter against
    // another

    emit graphRequired(mParameterActions.value(qobject_cast<QAction *>(sender())),
                       mParameters.value(mPropertyEditor->currentProperty()));
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
