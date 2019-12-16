/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Simulation Experiment view information solvers widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "corecliutils.h"
#include "interfaces.h"
#include "simulation.h"
#include "simulationexperimentviewinformationsolverswidget.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationSolversWidgetData::SimulationExperimentViewInformationSolversWidgetData(const QMap<QString, SolverInterface *> &pSolversInterfaces,
                                                                                                           Core::Property *pSolversProperty,
                                                                                                           Core::Property *pSolversListProperty,
                                                                                                           const QMap<QString, Core::Properties> &pSolversProperties) :
    mSolversInterfaces(pSolversInterfaces),
    mSolversProperty(pSolversProperty),
    mSolversListProperty(pSolversListProperty),
    mSolversProperties(pSolversProperties)
{
}

//==============================================================================

QMap<QString, SolverInterface *> SimulationExperimentViewInformationSolversWidgetData::solversInterfaces() const
{
    // Return our solvers interfaces

    return mSolversInterfaces;
}

//==============================================================================

Core::Property * SimulationExperimentViewInformationSolversWidgetData::solversProperty() const
{
    // Return our solvers property

    return mSolversProperty;
}

//==============================================================================

Core::Property * SimulationExperimentViewInformationSolversWidgetData::solversListProperty() const
{
    // Return our solvers list property

    return mSolversListProperty;
}

//==============================================================================

QMap<QString, Core::Properties> SimulationExperimentViewInformationSolversWidgetData::solversProperties() const
{
    // Return our solvers properties

    return mSolversProperties;
}

//==============================================================================

SimulationExperimentViewInformationSolversWidget::SimulationExperimentViewInformationSolversWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent)
{
    // Remove all our properties

    removeAllProperties();

    // Add properties for our different solvers

    mOdeSolverData = addSolverProperties(Solver::Type::Ode);
    mNlaSolverData = addSolverProperties(Solver::Type::Nla);

    // Show/hide the relevant properties

    solverChanged(mOdeSolverData, mOdeSolverData->solversListProperty()->value());

    if (mNlaSolverData != nullptr) {
        solverChanged(mNlaSolverData, mNlaSolverData->solversListProperty()->value());
    }

    // Expand all our properties

    expandAll();

    // Keep track of changes to list properties

    connect(this, &SimulationExperimentViewInformationSolversWidget::propertyChanged,
            this, QOverload<Core::Property *>::of(&SimulationExperimentViewInformationSolversWidget::solverChanged));
}

//==============================================================================

SimulationExperimentViewInformationSolversWidget::~SimulationExperimentViewInformationSolversWidget()
{
    // Delete some internal objects

    delete mOdeSolverData;
    delete mNlaSolverData;
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::retranslateUi()
{
    // Update our property names

    mOdeSolverData->solversProperty()->setName(tr("ODE solver"));
    mOdeSolverData->solversListProperty()->setName(tr("Name"));

    mOdeSolverData->solversListProperty()->setEmptyListValue(tr("None available"));

    if (mNlaSolverData != nullptr) {
        mNlaSolverData->solversProperty()->setName(tr("NLA solver"));
        mNlaSolverData->solversListProperty()->setName(tr("Name"));

        mNlaSolverData->solversListProperty()->setEmptyListValue(tr("None available"));
    }

    // Update the name of our various properties, should they have a description
    // associated with them
    // Note: this is effectively to have the description of our solvers'
    //       properties properly updated...

    for (auto property : allProperties()) {
        if (mDescriptions.contains(property)) {
            // The property has a description associated with it, so retrieve
            // the version, if any, which corresponds to our current locale

            Descriptions descriptions = mDescriptions.value(property);
            QString description = descriptions.value(Core::locale());

            if (description.isEmpty()) {
                // No description exists for the current locale, so retrieve the
                // English description (which, hopefully, exists)

                description = descriptions.value("en");
            }

            // Set the name of the property to the description

            property->setName(description);
        }
    }

    // Default retranslation
    // Note: we must do it last since we set the empty list value of some
    //       properties above...

    PropertyEditorWidget::retranslateUi();
}

//==============================================================================

SimulationExperimentViewInformationSolversWidgetData * SimulationExperimentViewInformationSolversWidget::addSolverProperties(Solver::Type pSolverType)
{
    // Retrieve the name of the solvers which type is the one in which we are
    // interested

    QMap<QString, SolverInterface *> solversInterfaces = QMap<QString, SolverInterface *>();
    Core::Property *solversProperty = nullptr;
    Core::Property *solversListProperty = nullptr;
    QStringList solversNames = QStringList();
    QMap<QString, Core::Properties> solversProperties = QMap<QString, Core::Properties>();

    for (auto solverInterface : Core::solverInterfaces()) {
        if (solverInterface->solverType() == pSolverType) {
            // Keep track of the solver's interface

            solversInterfaces.insert(solverInterface->solverName(), solverInterface);

            // Keep track of the solver's name

            solversNames << solverInterface->solverName();

            // Add our solvers section property and list property for our
            // solvers, if needed

            if (solversProperty == nullptr) {
                solversProperty = addSectionProperty();

                solversListProperty = addListProperty(solversProperty);
            }

            // Add the solver's properties

            Core::Property *property = nullptr;
            Core::Properties properties = Core::Properties();

            for (const auto &solverInterfaceProperty : solverInterface->solverProperties()) {
                // Add the solver's property and set its default value

                switch (solverInterfaceProperty.type()) {
                case Solver::Property::Type::Boolean:
                    property = addBooleanProperty(solverInterfaceProperty.defaultValue().toBool(),
                                                  solversProperty);

                    break;
                case Solver::Property::Type::Integer:
                    property = addIntegerProperty(solverInterfaceProperty.defaultValue().toInt(),
                                                  solversProperty);

                    break;
                case Solver::Property::Type::IntegerGe0:
                    property = addIntegerGe0Property(solverInterfaceProperty.defaultValue().toInt(),
                                                     solversProperty);

                    break;
                case Solver::Property::Type::IntegerGt0:
                    property = addIntegerGt0Property(solverInterfaceProperty.defaultValue().toInt(),
                                                     solversProperty);

                    break;
                case Solver::Property::Type::Double:
                    property = addDoubleProperty(solverInterfaceProperty.defaultValue().toDouble(),
                                                 solversProperty);

                    break;
                case Solver::Property::Type::DoubleGe0:
                    property = addDoubleGe0Property(solverInterfaceProperty.defaultValue().toDouble(),
                                                    solversProperty);

                    break;
                case Solver::Property::Type::DoubleGt0:
                    property = addDoubleGt0Property(solverInterfaceProperty.defaultValue().toDouble(),
                                                    solversProperty);

                    break;
                case Solver::Property::Type::List:
                    property = addListProperty(solverInterfaceProperty.listValues(),
                                               solverInterfaceProperty.defaultValue().toString(),
                                               solversProperty);

                    break;
                }

                property->setId(solverInterfaceProperty.id());

                // Set the solver's property's 'unit', if needed

                if (solverInterfaceProperty.hasVoiUnit()) {
                    property->setUnit("???");
                    // Note: to assign a non-empty string to our unit item is
                    //       just a way for us to make sure that the property's
                    //       will get initialised (see setPropertiesUnit())...
                }

                // Keep track of the solver's property

                properties << property;

                // Keep track of the solver's property's descriptions

                mDescriptions.insert(property, solverInterfaceProperty.descriptions());
            }

            // Keep track of the solver's properties

            solversProperties.insert(solverInterface->solverName(), properties);
        }
    }

    // Check whether we have at least one solver

    if (solversListProperty != nullptr) {
        // We have a solvers list property, which means that we have at least
        // one solver, so sort our list

        solversNames.sort(Qt::CaseInsensitive);

        // Assign the list of solvers to our list property

        solversListProperty->setListValues(solversNames);

        // Create and return our solver data

        return new SimulationExperimentViewInformationSolversWidgetData(solversInterfaces,
                                                                        solversProperty,
                                                                        solversListProperty,
                                                                        solversProperties);
    }

    return nullptr;
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::setPropertiesUnit(SimulationExperimentViewInformationSolversWidgetData *pSolverData,
                                                                         const QString &pVoiUnit)
{
    // Make sure that we have some solver's data

    if (pSolverData == nullptr) {
        return;
    }

    // Go through the solvers' properties and set the unit of the relevant ones

    for (const auto &properties : pSolverData->solversProperties()) {
        for (auto property : properties) {
            if (!property->unit().isEmpty()) {
                property->setUnit(pVoiUnit);
            }
        }
    }
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::initialize(SimulationSupport::Simulation *pSimulation)
{
    // Make sure that the CellML file runtime is valid

    if (pSimulation->runtime()->isValid()) {
        // Show/hide the ODE/NLA solver information

        mOdeSolverData->solversProperty()->setVisible(true);

        if (mNlaSolverData != nullptr) {
            mNlaSolverData->solversProperty()->setVisible(pSimulation->runtime()->needNlaSolver());
        }
    }

    // Set the unit of our different properties, if needed

    QString voiUnit = pSimulation->runtime()->voi()->unit();

    setPropertiesUnit(mOdeSolverData, voiUnit);
    setPropertiesUnit(mNlaSolverData, voiUnit);

    // Initialise our simulation's NLA solver's properties, so that we can
    // properly reset our simulation the first time round

    if (mNlaSolverData != nullptr) {
        pSimulation->data()->setNlaSolverName(mNlaSolverData->solversListProperty()->value(), false);

        for (auto property : mNlaSolverData->solversProperties().value(pSimulation->data()->nlaSolverName())) {
            // Note: we pass false to variantValue() because we want to retrieve
            //       the value of list properties as a string rather than an
            //       index...

            pSimulation->data()->setNlaSolverProperty(property->id(),
                                                      property->variantValue(false),
                                                      false);
        }
    }
}

//==============================================================================

QStringList SimulationExperimentViewInformationSolversWidget::odeSolvers() const
{
    // Return the available ODE solvers, if any

    return mOdeSolverData->solversListProperty()->listValues();
}

//==============================================================================

QStringList SimulationExperimentViewInformationSolversWidget::nlaSolvers() const
{
    // Return the available NLA solvers, if any

    return (mNlaSolverData != nullptr)?mNlaSolverData->solversListProperty()->listValues():QStringList();
}

//==============================================================================

SimulationExperimentViewInformationSolversWidgetData * SimulationExperimentViewInformationSolversWidget::odeSolverData() const
{
    // Return our ODE solver data

    return mOdeSolverData;
}

//==============================================================================

SimulationExperimentViewInformationSolversWidgetData * SimulationExperimentViewInformationSolversWidget::nlaSolverData() const
{
    // Return our NLA solver data

    return mNlaSolverData;
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::updateSolverGui(SimulationExperimentViewInformationSolversWidgetData *pSolverData)
{
    // Make sure that we have some solver data

    if (pSolverData == nullptr) {
        return;
    }

    // Make sure that we have a solver

    QString solverName = pSolverData->solversListProperty()->value();
    SolverInterface *solverInterface = pSolverData->solversInterfaces().value(solverName);

    if (solverInterface == nullptr) {
        return;
    }

    // Retrieve our solver properties values

    Core::Properties solverProperties = pSolverData->solversProperties().value(solverName);
    QMap<QString, QString> solverPropertiesValues = QMap<QString, QString>();

    for (auto solverProperty : solverProperties) {
        solverPropertiesValues.insert(solverProperty->id(),solverProperty->value());
    }

    // Ask our solver interface to tell us about the visibility of its
    // properties based on their values

    QMap<QString, bool> solverPropertiesVisibility = solverInterface->solverPropertiesVisibility(solverPropertiesValues);

    // Update the visilibity of our solver properties
    // Note: if there is no visibility information for a given solver property,
    //       then it means that it should be visible...

    for (auto solverProperty : solverProperties) {
        solverProperty->setVisible(solverPropertiesVisibility.value(solverProperty->id(), true));
    }
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::updateGui(SimulationExperimentViewInformationSolversWidgetData *pSolverData)
{
    // Update our solver(s) properties visibility

    if ((pSolverData == nullptr) || (pSolverData == mOdeSolverData)) {
        updateSolverGui(mOdeSolverData);
    }

    if ((mNlaSolverData != nullptr) && ((pSolverData == nullptr) || (pSolverData == mNlaSolverData))) {
        updateSolverGui(mNlaSolverData);
    }
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::solverChanged(SimulationExperimentViewInformationSolversWidgetData *pSolverData,
                                                                     const QString &pSolverName)
{
    // Make sure that we have some solver data

    if (pSolverData == nullptr) {
        return;
    }

    // Go through the different properties for the given type of solver and
    // show/hide whatever needs showing/hiding

    QMap<QString, Core::Properties> solversProperties = pSolverData->solversProperties();

    for (auto solverProperties = solversProperties.constBegin(),
              solverPropertiesEnd = solversProperties.constEnd();
         solverProperties != solverPropertiesEnd; ++solverProperties) {
        bool solverPropertiesVisible = solverProperties.key() == pSolverName;

        for (auto solverProperty : solverProperties.value()) {
            solverProperty->setVisible(solverPropertiesVisible);
        }
    }
}

//==============================================================================

void SimulationExperimentViewInformationSolversWidget::solverChanged(Core::Property *pProperty)
{
    // Try, for the ODE/NLA solvers list property, to handle the change in the
    // list property
    // Note: the ODE/NLA solvers list property is our first property, hence we
    //       make sure that its row number is equal to zero (in case there is
    //       one or several other list properties, as is the case for the CVODE
    //       solver)...

    if (pProperty->row() == 0) {
        solverChanged((pProperty == mOdeSolverData->solversListProperty())?
                          mOdeSolverData:
                          mNlaSolverData,
                      pProperty->value());
    }
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
