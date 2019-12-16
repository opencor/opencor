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
// Graph panels widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "graphpanelwidget.h"
#include "graphpanelwidgetglobal.h"
#include "splitterwidget.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelsWidget : public Core::SplitterWidget
{
    Q_OBJECT

public:
    explicit GraphPanelsWidget(QWidget *pParent);

    void retranslateUi() override;

    void initialize(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties = GraphPanelWidgetProperties());

    GraphPanelWidgets graphPanels() const;
    GraphPanelWidget * activeGraphPanel() const;

    GraphPanelWidget * addGraphPanel(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties = GraphPanelWidgetProperties(),
                                     bool pActive = true);

    bool removeCurrentGraphPanel(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties = GraphPanelWidgetProperties());
    void removeAllGraphPanels(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties = GraphPanelWidgetProperties());

    void setActiveGraphPanel(GraphPanelWidget *pGraphPanel);

    QIntList sizes() const;
    void setSizes(const QIntList &pSizes);

private:
    GraphPanelWidgets mGraphPanels;

    GraphPanelWidget *mActiveGraphPanel = nullptr;

    QAction *mSynchronizeXAxisAction;
    QAction *mSynchronizeYAxisAction;

    QIntList mInternalSizes;
    bool mUseInternalSizes = true;

    bool removeGraphPanel(GraphPanelWidget *pGraphPanel);

signals:
    void graphPanelAdded(GraphPanelWidget *pGraphPanel,
                         const GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                         bool pActive);
    void graphPanelRemoved(GraphPanelWidget *pGraphPanel);

    void graphPanelActivated(GraphPanelWidget *pGraphPanel);

    void graphAdded(GraphPanelWidget *pGraphPanel, GraphPanelPlotGraph *pGraph,
                    const GraphPanelPlotGraphProperties &pGraphProperties);
    void graphsRemoved(GraphPanelWidget *pGraphPanel,
                       const GraphPanelPlotGraphs &pGraphs);

private slots:
    void stopUsingInternalSizes();

    void updateGraphPanels(GraphPanelWidget *pGraphPanel);

    void synchronizeXAxis();
    void synchronizeYAxis();
};

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
