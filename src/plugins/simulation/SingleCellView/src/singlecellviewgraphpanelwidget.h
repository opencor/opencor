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
// Single Cell view graph panel widget
//==============================================================================

#pragma once

//==============================================================================

#include "singlecellviewgraphpanelplotwidget.h"
#include "widget.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class GraphPanelWidget;

//==============================================================================

typedef QList<GraphPanelWidget *> GraphPanelWidgets;

//==============================================================================

class GraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                              QWidget *pParent);
    ~GraphPanelWidget();

    virtual void retranslateUi();

    bool isActive() const;
    void setActive(const bool &pActive);

    GraphPanelPlotWidget * plot() const;

    SingleCellViewGraphPanelPlotGraphs graphs() const;

    void addGraph(GraphPanelPlotGraph *pGraph);
    void removeGraphs(const SingleCellViewGraphPanelPlotGraphs &pGraphs);
    void removeAllGraphs();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFrame *mMarker;
    GraphPanelPlotWidget *mPlot;

    bool mActive;

    void updateMarkerColor();

Q_SIGNALS:
    void activated(GraphPanelWidget *pGraphPanel);
    void inactivated(GraphPanelWidget *pGraphPanel);

    void graphAdded(GraphPanelWidget *pGraphPanel,
                    GraphPanelPlotGraph *pGraph);
    void graphsRemoved(GraphPanelWidget *pGraphPanel,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
