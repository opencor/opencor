/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view information parameters widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "simulation.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewsimulationwidget.h"

//==============================================================================

#include <QContextMenuEvent>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationParametersWidget::SimulationExperimentViewInformationParametersWidget(QWidget *pParent) :
    PropertyEditorWidget(false, pParent),
    mPlotAgainstVoiMenuAction(nullptr),
    mPlotAgainstMenu(nullptr),
    mParameters(QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mParameterActions(QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *>()),
    mSimulation(nullptr),
    mNeedClearing(false),
    mVoiAccessible(false)
{
    // Create our context menu

    mContextMenu = new QMenu(this);

    // Keep track of when the user changes a property value

    connect(this, &Core::PropertyEditorWidget::propertyChanged,
            this, &SimulationExperimentViewInformationParametersWidget::propertyChanged);
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::retranslateContextMenu()
{
    // Retranslate our context menu, if it exists / has been populated

    if (mPlotAgainstVoiMenuAction)
        mPlotAgainstVoiMenuAction->setText(tr("Plot Against Variable Of Integration"));

    if (mPlotAgainstMenu)
        mPlotAgainstMenu->menuAction()->setText(tr("Plot Against"));
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::retranslateUi()
{
    // Default retranslation

    PropertyEditorWidget::retranslateUi();

    // Retranslate our context menu

    retranslateContextMenu();

    // Retranslate the extra info of all our parameters

    updateExtraInfos();
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Make sure that we have a current property

    Core::Property *crtProperty = currentProperty();

    if (!crtProperty)
        return;

    // Make sure that our current property is not a section

    if (crtProperty->type() == Core::Property::Section)
        return;

    // Generate and show the context menu

    mContextMenu->exec(pEvent->globalPos());
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::initialize(SimulationSupport::Simulation *pSimulation,
                                                                     bool pReloadingView)
{
    // Keep track of the simulation

    mSimulation = pSimulation;

    // First clear ourselves, if needed

    if (mNeedClearing) {
        clear();

        mNeedClearing = false;
    }

    // Check whether our model's VOI is among our model's parameters (i.e. it is
    // defined in the main CellML file)

    CellMLSupport::CellmlFileRuntime *runtime = pSimulation->runtime();

    mVoiAccessible = runtime->parameters().contains(runtime->voi());

    // Retranslate our core self, if needed
    // Note: part of reloading ourselves consists of finalising ourselves, which
    //       means clearing all of our contents including our header labels. So,
    //       we need to 'retranslate' them otherwise they will read "1", "2",
    //       "3", etc.

    if (pReloadingView)
        PropertyEditorWidget::retranslateUi();

    // Populate our property editor and context menu

    populateModel(runtime);
    populateContextMenu(runtime);

    // Keep track of when some of the model's data has changed

    connect(pSimulation->data(), &SimulationSupport::SimulationData::updated,
            this, &SimulationExperimentViewInformationParametersWidget::updateParameters);
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::finalize()
{
    // Clear ourselves, as well as our context menu, parameters and parameter
    // actions

    mNeedClearing = true;

    mContextMenu->clear();

    mParameters.clear();
    mParameterActions.clear();
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::updateParameters(double pCurrentPoint)
{
    // Update our data

    foreach (Core::Property *property, allProperties()) {
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
    }

    // Check whether any of our properties has actually been modified

    mSimulation->data()->checkForModifications();
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::propertyChanged(Core::Property *pProperty)
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

QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> SimulationExperimentViewInformationParametersWidget::parameters() const
{
    // Return our parameters

    return mParameters;
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::populateModel(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Populate our property editor with the parameters

    QString componentHierarchy = QString();
    Core::Property *sectionProperty = nullptr;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new section hierarchy for our 'new' component, reusing
            // existing sections, whenever possible

            Core::Property *parentSectionProperty = nullptr;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a section for our current
                // component

                sectionProperty = nullptr;

                if (parentSectionProperty) {
                    // We have a parent section, so go through its children and
                    // retrieve the one for our current component

                    foreach (QObject *object, parentSectionProperty->children()) {
                        Core::Property *property = qobject_cast<Core::Property *>(object);

                        if (    property
                            &&  (property->type() == Core::Property::Section)
                            && !property->name().compare(component)) {
                            sectionProperty = property;

                            break;
                        }
                    }
                } else {
                    // We don't have a parent section, so go through our
                    // properties and retrieve the one for our current component

                    foreach (Core::Property *property, properties()) {
                        if (    (property->type() == Core::Property::Section)
                            && !property->name().compare(component)) {
                            sectionProperty = property;

                            break;
                        }
                    }
                }

                // Create a new section for our current component, if none could
                // be found

                if (!sectionProperty)
                    sectionProperty = addSectionProperty(component, parentSectionProperty);

                // Get ready for the next component in our component hierarchy

                parentSectionProperty = sectionProperty;
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
        property->setIcon(CellMLSupport::CellmlFileRuntimeParameter::icon(parameter->type()));
        property->setName(parameter->formattedName(), false);
        property->setUnit(parameter->formattedUnit(pRuntime->voi()->unit()), false);

        // Keep track of the link between our property value and parameter

        mParameters.insert(property, parameter);
    }

    // Update (well, set here) the extra info of all our parameters

    updateExtraInfos();

    // Expand all our properties

    expandAll();
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::populateContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime)
{
    // Create our two main menu items

    if (mVoiAccessible)
        mPlotAgainstVoiMenuAction = mContextMenu->addAction(QString());

    mPlotAgainstMenu = new QMenu(mContextMenu);

    mContextMenu->addMenu(mPlotAgainstMenu);

    // Initialise our two main menu items

    retranslateContextMenu();

    // Create a connection to handle the graph requirement against our VOI, and
    // keep track of the parameter associated with our first main menu item

    if (mVoiAccessible) {
        connect(mPlotAgainstVoiMenuAction, &QAction::triggered,
                this, &SimulationExperimentViewInformationParametersWidget::emitGraphRequired);

        mParameterActions.insert(mPlotAgainstVoiMenuAction, pRuntime->voi());
    }

    // Populate our context menu with the parameters

    QString componentHierarchy = QString();
    QMenu *componentMenu = nullptr;

    foreach (CellMLSupport::CellmlFileRuntimeParameter *parameter, pRuntime->parameters()) {
        // Check whether the current parameter is in the same component
        // hierarchy as the previous one

        QString crtComponentHierarchy = parameter->formattedComponentHierarchy();

        if (crtComponentHierarchy.compare(componentHierarchy)) {
            // The current parameter is in a different component hierarchy, so
            // create a new menu hierarchy for our 'new' component, reusing
            // existing menus, whenever possible

            QMenu *menu = mPlotAgainstMenu;

            foreach (const QString &component, parameter->componentHierarchy()) {
                // Check whether we already have a menu for our current
                // component

                componentMenu = nullptr;

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

        QAction *parameterAction = componentMenu->addAction(CellMLSupport::CellmlFileRuntimeParameter::icon(parameter->type()),
                                                            parameter->formattedName());

        // Create a connection to handle the graph requirement against our
        // parameter

        connect(parameterAction, &QAction::triggered,
                this, &SimulationExperimentViewInformationParametersWidget::emitGraphRequired);

        // Keep track of the parameter associated with our parameter action

        mParameterActions.insert(parameterAction, parameter);
    }
}

//==============================================================================

void SimulationExperimentViewInformationParametersWidget::updateExtraInfos()
{
    // Update the extra info of all our properties

    foreach (Core::Property *property, allProperties()) {
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

void SimulationExperimentViewInformationParametersWidget::emitGraphRequired()
{
    // Let people know that we want to plot the current parameter against
    // another

    emit graphRequired(mParameterActions.value(qobject_cast<QAction *>(sender())),
                       mParameters.value(currentProperty()));
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
