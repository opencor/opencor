/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewsimulation.h"
#include "singlecellviewsimulationwidget.h"

//==============================================================================

#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationParametersWidget::SingleCellViewInformationParametersWidget(QWidget *pParent) :
    PropertyEditorWidget(false, pParent),
    mParameters(QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mSimulation(0)
{
    // Create our context menu

    mContextMenu = new QMenu(this);

    // We want our own context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(propertyEditorContextMenu(const QPoint &)));

    // Keep track of when the user changes a property value

    connect(this, SIGNAL(propertyChanged(Core::Property *)),
            this, SLOT(propertyChanged(Core::Property *)));
}

//==============================================================================

void SingleCellViewInformationParametersWidget::retranslateContextMenu()
{
    // Retranslate our context menu, in case it has been populated

    if (mContextMenu->actions().count() >= 2) {
        mContextMenu->actions()[0]->setText(tr("Plot Against Variable of Integration"));
        mContextMenu->actions()[1]->setText(tr("Plot Against"));
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::retranslateUi()
{
    // Default retranslation

    PropertyEditorWidget::retranslateUi();

    // Retranslate our context menu

    retranslateContextMenu();

    // Retranslate the extra info of all our parameters

    updateExtraInfos();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::initialize(SingleCellViewSimulation *pSimulation,
                                                           const bool &pReloadingView)
{
    // Keep track of the simulation

    mSimulation = pSimulation;

    // Retranslate our core self, if needed
    // Note: part of reloading ourselves consists of finalising ourselves, which
    //       means clearing all of our contents including our header labels. So,
    //       we need to 'retranslate' them otherwise they will read "1", "2",
    //       "3", etc.

    if (pReloadingView)
        PropertyEditorWidget::retranslateUi();

    // Populate our property editor and context menu

    populateModel(pSimulation->runtime());
    populateContextMenu(pSimulation->runtime());

    // Keep track of when some of the model's data has changed

    connect(pSimulation->data(), SIGNAL(updated(const double &)),
            this, SLOT(updateParameters(const double &)));
}

//==============================================================================

void SingleCellViewInformationParametersWidget::finalize()
{
    // Clear ourselves, as well as our context menu, parameters and parameter
    // actions

    clear();

    mContextMenu->clear();

    mParameters.clear();
    mParameterActions.clear();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::updateParameters(const double &pCurrentPoint,
                                                                 const bool &pProcessEvents)
{
    // Update our data

    foreach (Core::Property *property, properties()) {
        CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(property);

        if (parameter) {
            switch (parameter->type()) {
            case CellMLSupport::CellmlFileRuntimeParameter::Voi:
                property->setDoubleValue(pCurrentPoint, false);

                break;
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
                // Not a relevant type, so do nothing

                break;
            }
        }

        // Make sure that we don't hang up the GUI unnecessarily
        // Note #1: this is useful when we run a model with loads of parameters
        //          since otherwise the simulation won't finish smoothly (see
        //          issue #656)...
        // Note #2: we don't want to process events all the time otherwise it
        //          would mean that whenever we move the mouse while editing a
        //          parameter, then the GUI would keep flashing because of the
        //          editor closing and then reopening repeatedly until we stop
        //          moving the mouse (see issue #733)...

        if (pProcessEvents)
            QCoreApplication::processEvents();
    }

    // Check whether any of our properties has actually been modified

    mSimulation->data()->checkForModifications();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyChanged(Core::Property *pProperty)
{
    // Update our simulation data

    CellMLSupport::CellmlFileRuntimeParameter *parameter = mParameters.value(pProperty);

    if (parameter) {
        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Constant:
            mSimulation->data()->constants()[parameter->index()] = pProperty->doubleValue();

            break;
        case CellMLSupport::CellmlFileRuntimeParameter::State:
            mSimulation->data()->states()[parameter->index()] = pProperty->doubleValue();

            break;
        default:
            // Not a relevant type, so do nothing

            ;
        }
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
    //          parameters (see issue #234 for more information), hence our
    //          passing false to mSimulation->data()->reset()...

    mSimulation->data()->reset(false);
}

//==============================================================================

QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> SingleCellViewInformationParametersWidget::parameters() const
{
    // Return our parameters

    return mParameters;
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateModel(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Populate our property editor with the parameters

    QString componentHierarchy = QString();
    Core::Property *sectionProperty = 0;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy.compare(componentHierarchy)) {
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

                if (section) {
                    // We have a section, so go through its children and keep
                    // track of its propeties that are a section

                    foreach (QObject *object, section->children()) {
                        Core::Property *property = qobject_cast<Core::Property *>(object);

                        if (   property
                            && (property->type() == Core::Property::Section)) {
                            subSections << property;
                        }
                    }
                } else {
                    // We don't have a section, so go through our properties and
                    // keep tack of those that are a section

                    foreach (Core::Property *property, properties()) {
                        if (property->type() == Core::Property::Section)
                            subSections << property;
                    }
                }

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
                    sectionProperty = addSectionProperty(component, section);

                // Get ready for the next component in our component hierarchy

                section = sectionProperty;
            }

            // Keep track of the new component hierarchy

            componentHierarchy = crtComponentHierarchy;
        }

        // Add the current parameter to the current section property, after
        // having retrieved its current value

        double propertyValue = 0.0;

        switch (parameter->type()) {
        case CellMLSupport::CellmlFileRuntimeParameter::Voi:
            propertyValue = mSimulation->data()->startingPoint();

            break;
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
            // Not a relevant type, so do nothing

            ;
        }

        Core::Property *property = addDoubleProperty(propertyValue, sectionProperty);

        property->setEditable(   (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::Constant)
                              || (parameter->type() == CellMLSupport::CellmlFileRuntimeParameter::State));

        property->setIcon(SingleCellViewSimulationWidget::parameterIcon(parameter->type()));

        property->setName(parameter->formattedName(), false);
        property->setUnit(parameter->formattedUnit(pRuntime->variableOfIntegration()->unit()), false);

        // Keep track of the link between our property value and parameter

        mParameters.insert(property, parameter);
    }

    // Update (well, set here) the extra info of all our parameters

    updateExtraInfos();

    // Expand all our properties

    expandAll();
}

//==============================================================================

void SingleCellViewInformationParametersWidget::populateContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Create our two main menu items

    QAction *voiAction = mContextMenu->addAction(QString());
    QMenu *plotAgainstMenu = new QMenu(mContextMenu);

    mContextMenu->addAction(plotAgainstMenu->menuAction());

    // Initialise our two main menu items

    retranslateContextMenu();

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

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *menu = plotAgainstMenu;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = 0;

                foreach (QObject *object, menu->children()) {
                    QMenu *subMenu = qobject_cast<QMenu *>(object);

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

            componentHierarchy = crtComponentHierarchy;
        }

        // Make sure that we have a 'current' component menu
        // Note: this should never happen, but we never know...

        if (!componentMenu)
            continue;

        // Add the current parameter to the 'current' component menu

        QAction *parameterAction = componentMenu->addAction(SingleCellViewSimulationWidget::parameterIcon(parameter->type()),
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
    // Update the extra info of all our properties

    foreach (Core::Property *property, properties()) {
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
                // Not a relevant type, so do nothing

                ;
            }

            property->setExtraInfo(parameterType);
        }
    }
}

//==============================================================================

void SingleCellViewInformationParametersWidget::propertyEditorContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Make sure that we have a current property

    Core::Property *crtProperty = currentProperty();

    if (!crtProperty)
        return;

    // Make sure that our current property is not a section

    if (crtProperty->type() == Core::Property::Section)
        return;

    // Generate and show the context menu

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void SingleCellViewInformationParametersWidget::emitGraphRequired()
{
    // Let people know that we want to plot the current parameter against
    // another

    emit graphRequired(mParameterActions.value(qobject_cast<QAction *>(sender())),
                       mParameters.value(currentProperty()));
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
