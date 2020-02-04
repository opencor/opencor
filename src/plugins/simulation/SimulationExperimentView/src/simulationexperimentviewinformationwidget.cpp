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
// Simulation Experiment view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "i18ninterface.h"
#include "simulationexperimentviewinformationparameterswidget.h"
#include "simulationexperimentviewinformationsimulationwidget.h"
#include "simulationexperimentviewinformationsolverswidget.h"
#include "simulationexperimentviewinformationwidget.h"

//==============================================================================

#include <QMenu>
#include <QSettings>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

SimulationExperimentViewInformationWidget::SimulationExperimentViewInformationWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                                     SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                                     QWidget *pParent) :
    QScrollArea(pParent),
    Core::CommonWidget(this)
{
    // Create and set our vertical layout

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins({});
    layout->setSpacing(0);

    setLayout(layout);

    // Remove the frame around our scroll area

    setFrameShape(QFrame::NoFrame);

    // Create our collapsible widget

    mCollapsibleWidget = new Core::CollapsibleWidget(this);

    mCollapsibleWidget->setObjectName("Collapsible");

    // Create our simulation widget

    mSimulationWidget = new SimulationExperimentViewInformationSimulationWidget(mCollapsibleWidget);

    mSimulationWidget->setObjectName("Simulation");

    // Create our solvers widget

    mSolversWidget = new SimulationExperimentViewInformationSolversWidget(mCollapsibleWidget);

    mSolversWidget->setObjectName("Solvers");

    // Create our graph panel and graphs widget

    mGraphPanelAndGraphsWidget = new SimulationExperimentViewInformationGraphPanelAndGraphsWidget(pViewWidget, pSimulationWidget, mCollapsibleWidget);

    mGraphPanelAndGraphsWidget->setObjectName("GraphPanelAndGraphs");

    // Create our parameters widget

    mParametersWidget = new SimulationExperimentViewInformationParametersWidget(mCollapsibleWidget);

    mParametersWidget->setObjectName("Parameters");

    // Add our different widgets to our collapsible widget

                                                      mCollapsibleWidget->addWidget(mSimulationWidget);
                                                      mCollapsibleWidget->addWidget(mSolversWidget);
    Core::CollapsibleHeaderWidget *graphPanelHeader = mCollapsibleWidget->addWidget(mGraphPanelAndGraphsWidget);
                                                      mCollapsibleWidget->addWidget(mParametersWidget, false);

    // Give more importance to our parameters widget than to our graph panel /
    // graphs widget
    // Note: indeed, our simulation and solvers widgets have a fixed size while
    //       both our parameters and graph panel / graphs widgets can take as
    //       much space as they want. Yet, we want our parameters widget to take
    //       more space since it's more 'important'...

    auto collapsibleWidgetLayout = static_cast<QVBoxLayout *>(mCollapsibleWidget->layout());

    collapsibleWidgetLayout->setStretchFactor(mGraphPanelAndGraphsWidget, 1);
    collapsibleWidgetLayout->setStretchFactor(mParametersWidget, 2);

    // Create and set a menu for our graph panel header

    auto menu = new QMenu(this);

    mGraphPanelAction = Core::newAction(true, graphPanelHeader);
    mGraphsAction = Core::newAction(true, graphPanelHeader);

    mGraphsAction->setChecked(true);

    auto settingsActionGroup = new QActionGroup(this);

    settingsActionGroup->addAction(mGraphPanelAction);
    settingsActionGroup->addAction(mGraphsAction);

    menu->addActions(settingsActionGroup->actions());

    graphPanelHeader->setMenu(menu);

    connect(mGraphPanelAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationWidget::grapPanelPropertyEditor);
    connect(mGraphsAction, &QAction::triggered,
            this, &SimulationExperimentViewInformationWidget::graphsPropertyEditor);

    // A connection to know when our graph panel / graphs panel changes modes

    connect(mGraphPanelAndGraphsWidget, &SimulationExperimentViewInformationGraphPanelAndGraphsWidget::graphPanelGraphsModeChanged,
            this, &SimulationExperimentViewInformationWidget::graphPanelGraphsModeChanged);

    // Add our collapsible widget to our layout

    layout->addWidget(mCollapsibleWidget);
}

//==============================================================================

void SimulationExperimentViewInformationWidget::retranslateUi()
{
    // Retranslate the different titles of our collapsible widget

    mCollapsibleWidget->setHeaderTitle(0, tr("Simulation"));
    mCollapsibleWidget->setHeaderTitle(1, tr("Solvers"));
    mCollapsibleWidget->setHeaderTitle(2, (mGraphPanelAndGraphsWidget->mode() == SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::GraphPanel)?
                                              tr("Graph Panel"):
                                              tr("Graphs"));
    mCollapsibleWidget->setHeaderTitle(3, tr("Parameters"));

    // Retranslate our graph panel actions

    I18nInterface::retranslateAction(mGraphPanelAction, tr("Graph Panel"),
                                     tr("Graph panel settings"));
    I18nInterface::retranslateAction(mGraphsAction, tr("Graphs"),
                                     tr("Graphs settings"));

    // Retranslate our simulation, solvers, graphs and parameters widgets

    mSimulationWidget->retranslateUi();
    mSolversWidget->retranslateUi();
    mGraphPanelAndGraphsWidget->retranslateUi();
    mParametersWidget->retranslateUi();
}

//==============================================================================

Core::CollapsibleWidget * SimulationExperimentViewInformationWidget::collapsibleWidget() const
{
    // Return our collapsible widget

    return mCollapsibleWidget;
}

//==============================================================================

SimulationExperimentViewInformationSimulationWidget * SimulationExperimentViewInformationWidget::simulationWidget() const
{
    // Return our simulation widget

    return mSimulationWidget;
}

//==============================================================================

SimulationExperimentViewInformationSolversWidget * SimulationExperimentViewInformationWidget::solversWidget() const
{
    // Return our solvers widget

    return mSolversWidget;
}

//==============================================================================

SimulationExperimentViewInformationGraphPanelAndGraphsWidget * SimulationExperimentViewInformationWidget::graphPanelAndGraphsWidget() const
{
    // Return our graph panel and graphs widget

    return mGraphPanelAndGraphsWidget;
}

//==============================================================================

SimulationExperimentViewInformationParametersWidget * SimulationExperimentViewInformationWidget::parametersWidget() const
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

void SimulationExperimentViewInformationWidget::finishEditing(bool pPausedSimulation)
{
    // Finish the editing of any of the information we support
    // Note: when a simulation is paused, the simulation and solvers widgets are
    //       disabled, so no editing to finish...

    if (!pPausedSimulation) {
        mSimulationWidget->finishEditing();
        mSolversWidget->finishEditing();
    }

    mGraphPanelAndGraphsWidget->finishEditing();
    mParametersWidget->finishEditing();
}

//==============================================================================

void SimulationExperimentViewInformationWidget::grapPanelPropertyEditor()
{
    // Switch to Graph Panel mode

    mGraphPanelAndGraphsWidget->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::GraphPanel);
}

//==============================================================================

void SimulationExperimentViewInformationWidget::graphsPropertyEditor()
{
    // Switch to Graphs mode

    mGraphPanelAndGraphsWidget->setMode(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::Graphs);
}

//==============================================================================

void SimulationExperimentViewInformationWidget::graphPanelGraphsModeChanged(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode pMode)
{
    // Our graph panel / graphs mode has changed, so update our corresponding
    // actions and update the title of our collapsible widget's third header's
    // title by retranslating ourselves

    mGraphPanelAction->setChecked(pMode == SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::GraphPanel);
    mGraphsAction->setChecked(pMode == SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::Graphs);

    retranslateUi();
}

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
