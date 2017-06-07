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

#include "coreguiutils.h"
#include "graphpanelwidget.h"

//==============================================================================

#include <QEvent>
#include <QHBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelWidget::GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                                   QAction *pSynchronizeXAxisAction,
                                   QAction *pSynchronizeYAxisAction,
                                   QWidget *pParent) :
    Widget(pParent)
{
    // We are not active by default

    mActive = false;

    // Create and set our horizontal layout

    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create, customise and add an inactive marker to our layout

    enum {
        MarkerWidth = 3
    };

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    layout->addWidget(mMarker);

    // Create and add a plot widget to our layout

    GraphPanelPlotWidgets neighbors = GraphPanelPlotWidgets();

    foreach (GraphPanelWidget *neighbor, pNeighbors)
        neighbors << neighbor->plot();

    mPlot = new GraphPanelPlotWidget(neighbors, pSynchronizeXAxisAction,
                                     pSynchronizeYAxisAction, this);

    layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    // Make sure that our marker's colour is properly intiialised

    updateMarkerColor();

    // Let our plot's neighbours know about our plot

    foreach (GraphPanelPlotWidget *neighbor, neighbors)
        neighbor->addNeighbor(mPlot);
}

//==============================================================================

GraphPanelWidget::GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                                   QWidget *pParent) :
    GraphPanelWidget(pNeighbors, 0, 0, pParent)
{
}

//==============================================================================

GraphPanelWidget::~GraphPanelWidget()
{
    // Let our plot's neighbours know that our plot is not going to be their
    // neighbour anymore

    foreach (GraphPanelPlotWidget *plot, mPlot->neighbors()) {
        if (plot != mPlot)
            plot->removeNeighbor(mPlot);
    }
}

//==============================================================================

void GraphPanelWidget::retranslateUi()
{
    // Retranslate our plot

    mPlot->retranslateUi();
}

//==============================================================================

void GraphPanelWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colour
    // used to highlight the active graph panel

    if (pEvent->type() == QEvent::PaletteChange)
        updateMarkerColor();
}

//==============================================================================

void GraphPanelWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QWidget::mousePressEvent(pEvent);

    // Activate/inactivate the graph panel
    // Note: we do it through setActive() because we want the graph panel to let
    //       people know that our active state has changed...

    setActive(true);
}

//==============================================================================

GraphPanelPlotWidget * GraphPanelWidget::plot() const
{
    // Return our plot

    return mPlot;
}

//==============================================================================

GraphPanelPlotGraphs GraphPanelWidget::graphs() const
{
    // Return all our plot's graphs

    return mPlot->graphs();
}

//==============================================================================

void GraphPanelWidget::addGraph(GraphPanelPlotGraph *pGraph)
{
    // Add the graph to our plot

    if (mPlot->addGraph(pGraph))
        emit graphAdded(this, pGraph);
}

//==============================================================================

void GraphPanelWidget::removeGraphs(const GraphPanelPlotGraphs &pGraphs)
{
    // Remove the graphs from our plot

    GraphPanelPlotGraphs graphs = GraphPanelPlotGraphs();

    foreach (GraphPanelPlotGraph *graph, pGraphs) {
        if (mPlot->removeGraph(graph))
            graphs << graph;
    }

    emit graphsRemoved(this, graphs);
}

//==============================================================================

void GraphPanelWidget::removeAllGraphs()
{
    // Remove all our graphs from our plot

    removeGraphs(graphs());
}

//==============================================================================

bool GraphPanelWidget::isActive() const
{
    // Return whether the graph panel is active

    return mActive;
}

//==============================================================================

void GraphPanelWidget::updateMarkerColor()
{
    // Update the marker's colour based on whether the graph panel is active

    mMarker->setStyleSheet("QFrame {"
                           "    color: "+QString(mActive?Core::highlightColor().name():Core::windowColor().name())+";"
                           "}");
}

//==============================================================================

void GraphPanelWidget::setActive(const bool &pActive)
{
    if (pActive == mActive)
        return;

    // Set the graph panel's active state

    mActive = pActive;

    // Update the marker's colour

    updateMarkerColor();

    // Let people know if the graph panel has been activated or inactivated

    if (pActive)
        emit activated(this);
    else
        emit inactivated(this);
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
