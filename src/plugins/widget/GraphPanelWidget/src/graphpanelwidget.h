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
// Graph panel widget
//==============================================================================

#pragma once

//==============================================================================

#include "graphpanelplotwidget.h"
#include "graphpanelwidgetglobal.h"
#include "widget.h"

//==============================================================================

#include <QList>

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

class GraphPanelWidget;

//==============================================================================

typedef QList<GraphPanelWidget *> GraphPanelWidgets;

//==============================================================================

class GRAPHPANELWIDGET_EXPORT GraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                              QAction *pSynchronizeXAxisAction,
                              QAction *pSynchronizeYAxisAction,
                              QWidget *pParent);
    explicit GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                              QWidget *pParent);
    ~GraphPanelWidget();

    virtual void retranslateUi();

    bool isActive() const;
    void setActive(const bool &pActive);

    GraphPanelPlotWidget * plot() const;

    GraphPanelPlotGraphs graphs() const;

    void addGraph(GraphPanelPlotGraph *pGraph);
    void removeGraphs(const GraphPanelPlotGraphs &pGraphs);
    void removeAllGraphs();

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    QFrame *mMarker;
    GraphPanelPlotWidget *mPlot;

    bool mActive;

    void constructor(const GraphPanelWidgets &pNeighbors,
                     QAction *pSynchronizeXAxisAction,
                     QAction *pSynchronizeYAxisAction);

    void updateMarkerColor();

signals:
    void activated(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);
    void inactivated(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);
};

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
