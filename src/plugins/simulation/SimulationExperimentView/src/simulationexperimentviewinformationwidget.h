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
// Simulation Experiment view information widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class CollapsibleWidget;
}   // namespace Core

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewInformationGraphsWidget;
class SimulationExperimentViewInformationParametersWidget;
class SimulationExperimentViewInformationSimulationWidget;
class SimulationExperimentViewInformationSolversWidget;
class SimulationExperimentViewSimulationWidget;
class SimulationExperimentViewWidget;

//==============================================================================

class SimulationExperimentViewInformationWidget : public QScrollArea,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewInformationWidget(SimulationExperimentViewWidget *pViewWidget,
                                                       SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                       QWidget *pParent);

    virtual void retranslateUi();

    Core::CollapsibleWidget * collapsibleWidget() const;

    SimulationExperimentViewInformationSimulationWidget * simulationWidget() const;
    SimulationExperimentViewInformationSolversWidget * solversWidget() const;
    SimulationExperimentViewInformationGraphsWidget * graphsWidget() const;
    SimulationExperimentViewInformationParametersWidget * parametersWidget() const;

    void finishEditing(const bool &pPausedSimulation);

private:
    Core::CollapsibleWidget *mCollapsibleWidget;

    SimulationExperimentViewInformationSimulationWidget *mSimulationWidget;
    SimulationExperimentViewInformationSolversWidget *mSolversWidget;
    SimulationExperimentViewInformationGraphsWidget *mGraphsWidget;
    SimulationExperimentViewInformationParametersWidget *mParametersWidget;
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
