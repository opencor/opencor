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
// Single Cell view contents widget
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

namespace SingleCellView {

//==============================================================================

class SingleCellViewInformationWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulationWidget;

//==============================================================================

class SingleCellViewContentsWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewContentsWidget(SingleCellViewPlugin *pPlugin,
                                          SingleCellViewSimulationWidget *pSimulationWidget,
                                          QWidget *pParent);

    virtual void retranslateUi();

    SingleCellViewInformationWidget * informationWidget() const;
    GraphPanelWidget::GraphPanelsWidget * graphPanelsWidget() const;

private:
    SingleCellViewInformationWidget *mInformationWidget;
    GraphPanelWidget::GraphPanelsWidget *mGraphPanelsWidget;

signals:
    void splitterMoved(const QIntList &pSizes);

private slots:
    void emitSplitterMoved();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
