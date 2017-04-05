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
// Simulation Experiment view information simulation widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewsimulation.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationSimulationWidget::SimulationExperimentViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent)
{
    // Populate our property editor

    mStartingPointProperty = addDoubleProperty(0.0);
    mEndingPointProperty = addDoubleProperty(1000.0);
    mPointIntervalProperty = addDoubleProperty(1.0);
}

//==============================================================================

void SimulationExperimentViewInformationSimulationWidget::retranslateUi()
{
    // Default retranslation

    PropertyEditorWidget::retranslateUi();

    // Update our property names

    mStartingPointProperty->setName(tr("Starting point"));
    mEndingPointProperty->setName(tr("Ending point"));
    mPointIntervalProperty->setName(tr("Point interval"));
}

//==============================================================================

void SimulationExperimentViewInformationSimulationWidget::initialize(SimulationExperimentViewSimulation *pSimulation)
{
    // Iniialise the unit of our different properties

    QString unit = pSimulation->runtime()->variableOfIntegration()->unit();

    mStartingPointProperty->setUnit(unit);
    mEndingPointProperty->setUnit(unit);
    mPointIntervalProperty->setUnit(unit);

    // Initialise our simulation's starting point so that we can then properly
    // reset our simulation the first time round

    pSimulation->data()->setStartingPoint(mStartingPointProperty->doubleValue(), false);
}

//==============================================================================

Core::Property * SimulationExperimentViewInformationSimulationWidget::startingPointProperty() const
{
    // Return our starting point property

    return mStartingPointProperty;
}

//==============================================================================

Core::Property * SimulationExperimentViewInformationSimulationWidget::endingPointProperty() const
{
    // Return our ending point property

    return mEndingPointProperty;
}

//==============================================================================

Core::Property * SimulationExperimentViewInformationSimulationWidget::pointIntervalProperty() const
{
    // Return our point interval property

    return mPointIntervalProperty;
}

//==============================================================================

double SimulationExperimentViewInformationSimulationWidget::startingPoint() const
{
    // Return our starting point

    return mStartingPointProperty->doubleValue();
}

//==============================================================================

double SimulationExperimentViewInformationSimulationWidget::endingPoint() const
{
    // Return our ending point

    return mEndingPointProperty->doubleValue();
}

//==============================================================================

double SimulationExperimentViewInformationSimulationWidget::pointInterval() const
{
    // Return our point interval

    return mPointIntervalProperty->doubleValue();
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
