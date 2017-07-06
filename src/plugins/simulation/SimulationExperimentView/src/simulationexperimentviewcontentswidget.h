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
// Simulation Experiment view contents widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "splitterwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace GraphPanelWidget {
    class GraphPanelsWidget;
}   // namespace GraphPanelWidget

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewInformationWidget;
class SimulationExperimentViewSimulationWidget;
class SimulationExperimentViewWidget;

//==============================================================================

class SimulationExperimentViewContentsWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewContentsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                    SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                    QWidget *pParent);

    virtual void retranslateUi();

    SimulationExperimentViewInformationWidget * informationWidget() const;
    GraphPanelWidget::GraphPanelsWidget * graphPanelsWidget() const;

private:
    SimulationExperimentViewInformationWidget *mInformationWidget;
    GraphPanelWidget::GraphPanelsWidget *mGraphPanelsWidget;

signals:
    void splitterMoved(const QIntList &pSizes);

private slots:
    void emitSplitterMoved();
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
