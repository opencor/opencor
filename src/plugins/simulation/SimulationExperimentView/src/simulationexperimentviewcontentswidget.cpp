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

#include "borderedwidget.h"
#include "graphpanelswidget.h"
#include "simulationexperimentviewcontentswidget.h"
#include "simulationexperimentviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewContentsWidget::SimulationExperimentViewContentsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                               SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                               QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Keep track of our movement

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(emitSplitterMoved()));

    // Create our information widget

    mInformationWidget = new SimulationExperimentViewInformationWidget(pViewWidget, pSimulationWidget, this);

    mInformationWidget->setObjectName("Information");

    // Create our graph panels widget

    mGraphPanelsWidget = new GraphPanelWidget::GraphPanelsWidget(this);

    mGraphPanelsWidget->setObjectName("GraphPanels");

    // Add our information and graph panels widgets to ourselves

    addWidget(new Core::BorderedWidget(mInformationWidget,
                                       false, false, true, true));
    addWidget(new Core::BorderedWidget(mGraphPanelsWidget,
                                       false, true, true, false));

    // Make our graph panels widget our focus proxy

    setFocusProxy(mGraphPanelsWidget);
}

//==============================================================================

void SimulationExperimentViewContentsWidget::retranslateUi()
{
    // Retranslate our information and graph panels widgets

    mInformationWidget->retranslateUi();
    mGraphPanelsWidget->retranslateUi();
}

//==============================================================================

SimulationExperimentViewInformationWidget * SimulationExperimentViewContentsWidget::informationWidget() const
{
    // Return our information widget

    return mInformationWidget;
}

//==============================================================================

GraphPanelWidget::GraphPanelsWidget * SimulationExperimentViewContentsWidget::graphPanelsWidget() const
{
    // Return our graph panels widget

    return mGraphPanelsWidget;
}

//==============================================================================

void SimulationExperimentViewContentsWidget::emitSplitterMoved()
{
    // Let people know that our splitter has been moved

    emit splitterMoved(sizes());
}

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
