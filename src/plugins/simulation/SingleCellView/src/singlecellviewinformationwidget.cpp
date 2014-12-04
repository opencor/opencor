/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view information widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreguiutils.h"
#include "singlecellviewinformationgraphswidget.h"
#include "singlecellviewinformationparameterswidget.h"
#include "singlecellviewinformationsimulationwidget.h"
#include "singlecellviewinformationsolverswidget.h"
#include "singlecellviewinformationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include "ui_singlecellviewinformationwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewInformationWidget::SingleCellViewInformationWidget(QWidget *pParent) :
    QScrollArea(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::SingleCellViewInformationWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Remove the frame around our scroll area

    setFrameShape(QFrame::NoFrame);

    // Create our collapsible widget

    mCollapsibleWidget = new Core::CollapsibleWidget(this);

    mCollapsibleWidget->setObjectName("Collapsible");

    // Create our simulation widget

    mSimulationWidget = new SingleCellViewInformationSimulationWidget(mCollapsibleWidget);

    mSimulationWidget->setObjectName("Simulation");

    // Create our solvers widget

    mSolversWidget = new SingleCellViewInformationSolversWidget(mCollapsibleWidget);

    mSolversWidget->setObjectName("Solvers");

    // Create our graphs widget

    mGraphsWidget = new SingleCellViewInformationGraphsWidget(mCollapsibleWidget);

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

    mGui->layout->addWidget(mCollapsibleWidget);
}

//==============================================================================

SingleCellViewInformationWidget::~SingleCellViewInformationWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellViewInformationWidget::retranslateUi()
{
    // Retranslate the whole widget

    mGui->retranslateUi(this);

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

void SingleCellViewInformationWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of our collapsible widget

    pSettings->beginGroup(mCollapsibleWidget->objectName());
        mCollapsibleWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our simulation widget

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our solvers widget

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our graphs widget

    pSettings->beginGroup(mGraphsWidget->objectName());
        mGraphsWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the settings of our parameters widget

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SingleCellViewInformationWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of our collapsible widget

    pSettings->beginGroup(mCollapsibleWidget->objectName());
        mCollapsibleWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our simulation widget

    pSettings->beginGroup(mSimulationWidget->objectName());
        mSimulationWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our solvers widget

    pSettings->beginGroup(mSolversWidget->objectName());
        mSolversWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our graphs widget

    pSettings->beginGroup(mGraphsWidget->objectName());
        mGraphsWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the settings of our parameters widget

    pSettings->beginGroup(mParametersWidget->objectName());
        mParametersWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

SingleCellViewInformationSimulationWidget * SingleCellViewInformationWidget::simulationWidget()
{
    // Return our simulation widget

    return mSimulationWidget;
}

//==============================================================================

SingleCellViewInformationSolversWidget * SingleCellViewInformationWidget::solversWidget()
{
    // Return our solvers widget

    return mSolversWidget;
}

//==============================================================================

SingleCellViewInformationGraphsWidget * SingleCellViewInformationWidget::graphsWidget()
{
    // Return our graphs widget

    return mGraphsWidget;
}

//==============================================================================

SingleCellViewInformationParametersWidget * SingleCellViewInformationWidget::parametersWidget()
{
    // Return our parameters widget

    return mParametersWidget;
}

//==============================================================================

void SingleCellViewInformationWidget::finishEditing()
{
    // Finish the editing of any of the information we support

    mSimulationWidget->finishEditing();
    mSolversWidget->finishEditing();
    mGraphsWidget->finishEditing();
    mParametersWidget->finishEditing();
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
