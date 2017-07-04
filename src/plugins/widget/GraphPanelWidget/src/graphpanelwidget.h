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
