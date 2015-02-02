/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "propertyeditorwidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QHeaderView>
#include <QMenu>
#include <QMetaType>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationParametersWidget::SingleCellViewInformationParametersWidget(QWidget *pParent) :
    QStackedWidget(pParent),
    Core::CommonWidget(pParent),
    mPropertyEditors(QMap<QString, Core::PropertyEditorWidget *>()),
    mPropertyEditor(0),
    mContextMenus(QMap<QString, QMenu *>()),
    mParameters(QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mColumnWidths(QIntList()),
    mFileName(QString()),
    mSimulation(0)
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

    pContextMenu->actions()[0]->setText(tr("Plot Against Variable of Integration"));
    pContextMenu->actions()[1]->setText(tr("Plot Against"));
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

    // Retranslate the extra info of all our parameters

    updateExtraInfos();
}

//==============================================================================

static const auto SettingsColumnWidths = QStringLiteral("ColumnWidths");

//==============================================================================

void SingleCellViewInformationParametersWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the width of each column of our property editors

    mColumnWidths = qVariantListToIntList(pSettings->value(SettingsColumnWidths, qIntListToVariantList(mColumnWidths)).toList());
}

//==============================================================================

void SingleCellViewInformationParametersWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the width of each column of our current property editor

    pSettings->setValue(SettingsColumnWidths, qIntListToVariantList(mColumnWidths));
}

//==============================================================================

void SingleCellViewInformationParametersWidget::initialize(const QString &pFileName,
                                                           CellMLSupport::CellmlFileRuntime *pRuntime,
                                                           SingleCellViewSimulation *pSimulation)
{
    // Keep track of the file name and simulation

    mFileName = pFileName;
    mSimulation = pSimulation;

    // Retrieve the property editor for the given file name or create one, if
    // none exists

    mPropertyEditor = mPropertyEditors.value(pFileName);

    if (!mPropertyEditor) {
        // No property editor exists for the given file name, so create one

        mPropertyEditor = new Core::PropertyEditorWidget(this);

        // Initialise our property editor's columns' width

        for (int i = 0, iMax = mColumnWidths.size(); i < iMax; ++i)
            mPropertyEditor->setColumnWidth(i, mColumnWidths[i]);

        // Populate our property editor

        populateModel(pRuntime);

        // Create and populate our context menu

        QMenu *contextMenu = new QMenu(this);

        populateContextMenu(contextMenu, pRuntime);

        // We want our own context menu for our property editor

        mPropertyEditor->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(mPropertyEditor, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(propertyEditorContextMenu(const QPoint &)));

        // Keep track of changes to columns' width

        connect(mPropertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

        // Keep track of when some of the model's data has changed

        connect(pSimulation->data(), SIGNAL(updated(const double &)),
                this, SLOT(updateParameters(const double &)));

        // Keep track of when the user changes a property value

        connect(mPropertyEditor, SIGNAL(propertyChanged(Core::Property *)),
                this, SLOT(propertyChanged(Core::Property *)));

        // Add our new property editor to ourselves

        addWidget(mPropertyEditor);

        // Keep track of our new property editor and context menu

        mPropertyEditors.insert(pFileName, mPropertyEditor);
        mContextMenus.insert(pFileName, contextMenu);
    }

    // Set our retrieved property editor as our current widget

    setCurrentWidget(mPropertyEditor);

    // Update the extra info of all our parameters
    // Note: this is in case the user changed the locale and then switched to a
    //       different file...

    updateExtraInfos();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::finalize(const QString &pFileName)
{
    // Remove track of our property editor and its context menu

    mPropertyEditors.remove(pFileName);
    mContextMenus.remove(pFileName);
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
            case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
                property->setDoubleValue(mSimulation->data()->constants()[parameter->index()], false);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Rate:
                property->setDoubleValue(mSimulation->data()->rates()[parameter->index()], false);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::State:
                property->setDoubleValue(mSimulation->data()->states()[parameter->index()], false);

                break;
            case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
                property->setDoubleValue(mSimulation->data()->algebraic()[parameter->index()], false);

                break;
            default:
                // CellMLSupport::CellmlFileRuntimeParameter::Voi

                property->setDoubleValue(pCurrentPoint, false);
            }
    }

    // Check whether any of our properties has actually been modified

    mSimulation->data()->checkForModifications();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyChanged(Core::Property *pProperty)
{
    // Update our simulation data

    CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(pProperty);

    if (parameter)
        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            mSimulation->data()->constants()[parameter->index()] = pProperty->doubleValue();

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            mSimulation->data()->states()[parameter->index()] = pProperty->doubleValue();

            break;
        default:
            // Not a type in which we are interested, so do nothing

            ;
        }

    // Recompute our 'computed constants' and 'variables'
    // Note #1: we would normally call
    //          mSimulation->data()->checkForModifications() after recomputing
    //          our 'computed constants' and 'variables, but the recomputation
    //          will eventually result in updateParameters() above to be called,
    //          which will check for modifications...
    // Note #2: some state variables may be considered as computed constants by
    //          the CellML API. This is fine when we need to initialise things,
    //          but not after the user has modified one or several model
    //          parameters (see https://github.com/opencor/opencor/issues/234
    //          for more insight on this issue), hence our passing false to
    //          mSimulation->data()->reset()...

    mSimulation->data()->reset(false);
}

//==============================================================================

void SingleCellViewInformationParametersWidget::finishEditing()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Ask our current property editor to finish the editing

    mPropertyEditor->finishEditing();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateModel(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Prevent ourselves from being updated (to avoid flickering)

    mPropertyEditor->setUpdatesEnabled(false);

    // Populate our property editor with the parameters

    QString componentHierarchy = QString();
    Core::Property *sectionProperty = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString currentComponentHierarchy = parameter->formattedComponentHierarchy();

        if (currentComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new section hierarchy for our 'new' component, reusing
            // existing sections, whenever possible

            Core::Property *section = 0;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a section for our current
                // component

                sectionProperty = 0;

                // Retrieve the sub-sections for the current section

                QList<Core::Property *> subSections = QList<Core::Property *>();

                if (section)
                    // We have a section, so go through its children and keep
                    // track of its propeties that are a section

                    foreach (QObject *object, section->children()) {
                        Core::Property *property = dynamic_cast<Core::Property *>(object);

                        if (   property
                            && (property->type() == Core::Property::Section)) {
                            subSections << property;
                        }
                    }
                else
                    // We don't have a section, so go through our property
                    // editor's properties and keep tack of those that are a
                    // section

                    foreach (Core::Property *property, mPropertyEditor->properties())
                        if (property->type() == Core::Property::Section)
                            subSections << property;

                // Go through the sub-sections and check if one of them is the
                // one we are after

                foreach (Core::Property *subSection, subSections) {
                    if (!subSection->name().compare(component)) {
                        sectionProperty = subSection;

                        break;
                    }
                }

                // Create a new section for our current component, if none could
                // be found

                if (!sectionProperty)
                    sectionProperty = mPropertyEditor->addSectionProperty(component, section);

                // Get ready for the next component in our component hierarchy

                section = sectionProperty;
            }

            // Keep track of the new component hierarchy

            componentHierarchy = currentComponentHierarchy;
        }

        // Add the current parameter to the current section property, after
        // having retrieved its current value

        double propertyValue = 0.0;

        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
        case CellMLSupport::CellmlFileRuntimeParameter::ComputedConstant:
            propertyValue = mSimulation->data()->constants()[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Rate:
            propertyValue = mSimulation->data()->rates()[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            propertyValue = mSimulation->data()->states()[parameter->index()];

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::Algebraic:
            propertyValue = mSimulation->data()->algebraic()[parameter->index()];

            break;
        default:
            // CellMLSupport::CellmlFileRuntimeParameter::Voi

            propertyValue = mSimulation->data()->startingPoint();
        }

        Core::Property *property = mPropertyEditor->addDoubleProperty(propertyValue, sectionProperty);

        property->setEditable(   (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)
                              || (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State));

        property->setIcon(SingleCellViewWidget::parameterIcon(parameter->type()));

        property->setName(parameter->formattedName());
        property->setUnit(parameter->formattedUnit(pRuntime->variableOfIntegration()->unit()));

        // Keep track of the link between our property value and parameter

        mParameters.insert(property, parameter);
    }

    // Update (well, set here) the extra info of all our parameters

    updateExtraInfos();

    // Expand all our properties

    mPropertyEditor->expandAll();

    // Allow ourselves to be updated again

    mPropertyEditor->setUpdatesEnabled(true);
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateContextMenu(QMenu *pContextMenu,
                                                                    CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Create our two main menu items

    QAction *voiAction = pContextMenu->addAction(QString());
    QMenu *plotAgainstMenu = new QMenu(pContextMenu);

    pContextMenu->addAction(plotAgainstMenu->menuAction());

    // Initialise our two main menu items

    retranslateContextMenu(pContextMenu);

    // Create a connection to handle the graph requirement against our variable
    // of integration

    connect(voiAction, SIGNAL(triggered(bool)),
            this, SLOT(emitGraphRequired()));

    // Keep track of the parameter associated with our first main menu item

    mParameterActions.insert(voiAction, pRuntime->variableOfIntegration());

    // Populate our context menu with the parameters

    QString componentHierarchy = QString();
    QMenu *componentMenu = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString currentComponentHierarchy = parameter->formattedComponentHierarchy();

        if (currentComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *menu = plotAgainstMenu;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = 0;

                foreach (QObject *object, menu->children()) {
                    QMenu *subMenu = dynamic_cast<QMenu *>(object);

                    if (    subMenu
                        && !subMenu->menuAction()->text().compare(component)) {
                        componentMenu = subMenu;

                        break;
                    }
                }

                // Create a new menu for our current component, if none could be
                // found

                if (!componentMenu) {
                    componentMenu = new QMenu(component, menu);

                    menu->addMenu(componentMenu);
                }

                // Get ready for the next component in our component hierarchy

                menu = componentMenu;
            }

            // Keep track of the new component hierarchy

            componentHierarchy = currentComponentHierarchy;
        }

        // Make sure that we have a 'current' component menu
        // Note: this should never happen, but we never know...

        if (!componentMenu)
            continue;

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(SingleCellViewWidget::parameterIcon(parameter->type()),
                                                            parameter->formattedName());

        // Create a connection to handle the graph requirement against our
        // parameter

        connect(parameterAction, SIGNAL(triggered(bool)),
                this, SLOT(emitGraphRequired()));

        // Keep track of the parameter associated with our model parameter
        // action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::updateExtraInfos()
{
    // Make sure that we have a property editor

    if (!mPropertyEditor)
        return;

    // Update the extra info of all our property editor's properties

    foreach (Core::Property *property, mPropertyEditor->properties()) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(property);

        if (parameter) {
            QString parameterType = QString();

            switch (parameter->type()) {
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
                // CellMLSupport::CellmlFileRuntimeParameter::Voi

                parameterType = tr("variable of integration");
            }

            property->setExtraInfo(parameterType);
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

    // Make that we have a current property

    Core::Property *currentProperty = mPropertyEditor->currentProperty();

    if (!currentProperty)
        return;

    // Make sure that our current property is not a section

    if (currentProperty->type() == Core::Property::Section)
        return;

    // Generate and show the context menu

    mContextMenus.value(mFileName)->exec(QCursor::pos());
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
        disconnect(propertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
                   this, SLOT(propertyEditorSectionResized(const int &, const int &, const int &)));

    // Update the column width of all our property editors

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        propertyEditor->header()->resizeSection(pLogicalIndex, pNewSize);

    // Keep track of the new column width

    mColumnWidths[pLogicalIndex] = pNewSize;

    // Re-allow all our property editors to respond to an updating of their
    // columns' width

    foreach (Core::PropertyEditorWidget *propertyEditor, mPropertyEditors)
        connect(propertyEditor->header(), SIGNAL(sectionResized(int, int, int)),
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
