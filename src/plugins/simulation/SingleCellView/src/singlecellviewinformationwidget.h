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

namespace SingleCellView {

//==============================================================================

class SingleCellViewInformationGraphsWidget;
class SingleCellViewInformationParametersWidget;
class SingleCellViewInformationSimulationWidget;
class SingleCellViewInformationSolversWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulationWidget;

//==============================================================================

class SingleCellViewInformationWidget : public QScrollArea,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationWidget(SingleCellViewPlugin *pPlugin,
                                             SingleCellViewSimulationWidget *pSimulationWidget,
                                             QWidget *pParent);

    virtual void retranslateUi();

    Core::CollapsibleWidget * collapsibleWidget() const;

    SingleCellViewInformationSimulationWidget * simulationWidget() const;
    SingleCellViewInformationSolversWidget * solversWidget() const;
    SingleCellViewInformationGraphsWidget * graphsWidget() const;
    SingleCellViewInformationParametersWidget * parametersWidget() const;

    void finishEditing(const bool &pPausedSimulation);

private:
    Core::CollapsibleWidget *mCollapsibleWidget;

    SingleCellViewInformationSimulationWidget *mSimulationWidget;
    SingleCellViewInformationSolversWidget *mSolversWidget;
    SingleCellViewInformationGraphsWidget *mGraphsWidget;
    SingleCellViewInformationParametersWidget *mParametersWidget;
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
