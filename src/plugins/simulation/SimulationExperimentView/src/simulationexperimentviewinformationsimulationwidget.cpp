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
// Simulation Experiment view information simulation widget
//==============================================================================

#include "cellmlfileruntime.h"
#include "simulation.h"
#include "simulationexperimentviewinformationsimulationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationSimulationWidget::SimulationExperimentViewInformationSimulationWidget(QWidget *pParent) :
    PropertyEditorWidget(true, pParent)
{
    // Populate our property editor

    mStartingPointProperty = addDoubleGe0Property(0.0);
    mEndingPointProperty = addDoubleGt0Property(1000.0);
    mPointIntervalProperty = addDoubleGt0Property(1.0);
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

void SimulationExperimentViewInformationSimulationWidget::initialize(SimulationSupport::Simulation *pSimulation)
{
    // Keep track of the simulation

    mSimulation = pSimulation;

    // Iniialise the unit of our different properties

    QString unit = pSimulation->runtime()->voi()->unit();

    mStartingPointProperty->setUnit(unit);
    mEndingPointProperty->setUnit(unit);
    mPointIntervalProperty->setUnit(unit);

    // Initialise our simulation's starting point so that we can then properly
    // reset our simulation the first time round

    pSimulation->data()->setStartingPoint(mStartingPointProperty->doubleValue(), false);

    // Keep track of when some of the model's data has changed

    connect(pSimulation->data(), &SimulationSupport::SimulationData::pointUpdated,
            this, &SimulationExperimentViewInformationSimulationWidget::updatePoints);
}

//==============================================================================

void SimulationExperimentViewInformationSimulationWidget::updatePoints()
{
    // Update our simulation points

    mStartingPointProperty->setDoubleValue(mSimulation->data()->startingPoint(), false);
    mEndingPointProperty->setDoubleValue(mSimulation->data()->endingPoint(), false);
    mPointIntervalProperty->setDoubleValue(mSimulation->data()->pointInterval(), false);
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

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
