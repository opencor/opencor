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
// Single cell view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewinformationwidget.h"

//==============================================================================

#include <QSettings>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationWidget::SingleCellViewInformationWidget(SingleCellViewPlugin *pPlugin,
                                                                 SingleCellViewSimulationWidget *pSimulationWidget,
                                                                 QWidget *pParent) :
    QScrollArea(pParent),
    Core::CommonWidget()
{
    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    // Remove the frame around our scroll area

    setFrameShape(QFrame::NoFrame);

    // Create our collapsible widget

    mCollapsibleWidget = new Core::CollapsibleWidget(this);

    mCollapsibleWidget->setObjectName("Collapsible");

    // Create our simulation widget

    mSimulationWidget = new SingleCellViewInformationSimulationWidget(mCollapsibleWidget);

    mSimulationWidget->setObjectName("Simulation");

    // Create our solvers widget

    mSolversWidget = new SingleCellViewInformationSolversWidget(pPlugin, mCollapsibleWidget);

    mSolversWidget->setObjectName("Solvers");

    // Create our graphs widget

    mGraphsWidget = new SingleCellViewInformationGraphsWidget(pPlugin, pSimulationWidget, mCollapsibleWidget);

    mGraphsWidget->setObjectName("Graphs");

    // Create our parameters widget

    mParametersWidget = new SingleCellViewInformationParametersWidget(mCollapsibleWidget);

    mParametersWidget->setObjectName("Parameters");

    // Add our simulation, solvers, graphs and parameters widgets to our
    // collapsible widget

    mCollapsibleWidget->addWidget(mSimulationWidget);
    mCollapsibleWidget->addWidget(mSolversWidget);
    mCollapsibleWidget->addWidget(mGraphsWidget);
    mCollapsibleWidget->addWidget(mParametersWidget, false);

    // Add our collapsible widget to our layout

    layout->addWidget(mCollapsibleWidget);
}

//==============================================================================

void SingleCellViewInformationWidget::retranslateUi()
{
    // Retranslate the different titles of our collapsible widget

    mCollapsibleWidget->setHeaderTitle(0, tr("Simulation"));
    mCollapsibleWidget->setHeaderTitle(1, tr("Solvers"));
    mCollapsibleWidget->setHeaderTitle(2, tr("Graphs"));
    mCollapsibleWidget->setHeaderTitle(3, tr("Parameters"));

    // Retranslate our simulation, solvers, graphs and parameters widgets

    mSimulationWidget->retranslateUi();
    mSolversWidget->retranslateUi();
    mGraphsWidget->retranslateUi();
    mParametersWidget->retranslateUi();
}

//==============================================================================

Core::CollapsibleWidget * SingleCellViewInformationWidget::collapsibleWidget() const
{
    // Return our collapsible widget

    return mCollapsibleWidget;
}

//==============================================================================

SingleCellViewInformationSimulationWidget * SingleCellViewInformationWidget::simulationWidget() const
{
    // Return our simulation widget

    return mSimulationWidget;
}

//==============================================================================

SingleCellViewInformationSolversWidget * SingleCellViewInformationWidget::solversWidget() const
{
    // Return our solvers widget

    return mSolversWidget;
}

//==============================================================================

SingleCellViewInformationGraphsWidget * SingleCellViewInformationWidget::graphsWidget() const
{
    // Return our graphs widget

    return mGraphsWidget;
}

//==============================================================================

SingleCellViewInformationParametersWidget * SingleCellViewInformationWidget::parametersWidget() const
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

void SingleCellViewInformationWidget::finishEditing(const bool &pPausedSimulation)
{
    // Finish the editing of any of the information we support
    // Note: when a simulation is paused, the simulation and solvers widgets are
    //       disabled, so no editing to finish...

    if (!pPausedSimulation) {
        mSimulationWidget->finishEditing();
        mSolversWidget->finishEditing();
    }

    mGraphsWidget->finishEditing();
    mParametersWidget->finishEditing();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
