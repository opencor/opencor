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
// Graph panels widget
//==============================================================================

#pragma once

//==============================================================================

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

    virtual void retranslateUi();

    void initialize();

    GraphPanelWidgets graphPanels() const;
    GraphPanelWidget * activeGraphPanel() const;

    GraphPanelWidget * addGraphPanel(const bool &pActive = true);

    bool removeCurrentGraphPanel();
    void removeAllGraphPanels();

    void setActiveGraphPanel(GraphPanelWidget *pGraphPanel);

private:
    GraphPanelWidgets mGraphPanels;

    GraphPanelWidget *mActiveGraphPanel;

    QAction *mSynchronizeXAxisAction;
    QAction *mSynchronizeYAxisAction;

    bool removeGraphPanel(GraphPanelWidget *pGraphPanel);

signals:
    void graphPanelAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                         const bool &pActive);
    void graphPanelRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void removeGraphPanelsEnabled(const bool &pEnabled);

    void graphPanelActivated(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

private slots:
    void updateGraphPanels(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void synchronizeXAxis();
    void synchronizeYAxis();
};

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
