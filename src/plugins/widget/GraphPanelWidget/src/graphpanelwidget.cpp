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

GraphPanelWidgetProperties::GraphPanelWidgetProperties(const QColor &pBackgroundColor,
                                                       const QColor &pForegroundColor,
                                                       bool pLegend) :
    mBackgroundColor(pBackgroundColor),
    mForegroundColor(pForegroundColor),
    mLegend(pLegend)
{
}

//==============================================================================

QColor GraphPanelWidgetProperties::backgroundColor() const
{
    // Return our background colour

    return mBackgroundColor;
}

//==============================================================================

QColor GraphPanelWidgetProperties::foregroundColor() const
{
    // Return our foreground colour

    return mForegroundColor;
}

//==============================================================================

bool GraphPanelWidgetProperties::legend() const
{
    // Return our legend

    return mLegend;
}

//==============================================================================

GraphPanelWidget::GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                                   QAction *pSynchronizeXAxisAction,
                                   QAction *pSynchronizeYAxisAction,
                                   QWidget *pParent) :
    Widget(pParent)
{
    // Create and set our horizontal layout

    auto layout = new QHBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create, customise and add an inactive marker to our layout
    // Note: we want the width of our marker to be 3 pixels wide. However, on
    //       macOS, to actually get a 3-pixel-wide marker, we need to make it 4
    //       pixels wide...!?

    enum {
#ifdef Q_OS_MAC
        MarkerWidth = 4
#else
        MarkerWidth = 3
#endif
    };

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    layout->addWidget(mMarker);

    // Create and add a plot widget to our layout

    GraphPanelPlotWidgets neighbors = GraphPanelPlotWidgets();

    for (auto neighbor : pNeighbors) {
        neighbors << neighbor->plot();
    }

    mPlot = new GraphPanelPlotWidget(neighbors, pSynchronizeXAxisAction,
                                     pSynchronizeYAxisAction, this);

    layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);

    // Make sure that our marker's colour is properly intiialised

    updateMarkerColor();

    // Let our plot's neighbours know about our plot

    for (auto neighbor : neighbors) {
        neighbor->addNeighbor(mPlot);
    }
}

//==============================================================================

GraphPanelWidget::GraphPanelWidget(const GraphPanelWidgets &pNeighbors,
                                   QWidget *pParent) :
    GraphPanelWidget(pNeighbors, nullptr, nullptr, pParent)
{
}

//==============================================================================

GraphPanelWidget::~GraphPanelWidget()
{
    // Let our plot's neighbours know that our plot is not going to be their
    // neighbour anymore

    for (auto plot : mPlot->neighbors()) {
        if (plot != mPlot) {
            plot->removeNeighbor(mPlot);
        }
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

    Core::Widget::changeEvent(pEvent);

    // Check whether the palette or our enabled state has changed and if so then
    // update the colour used to highlight the active graph panel

    if (   (pEvent->type() == QEvent::PaletteChange)
        || (pEvent->type() == QEvent::EnabledChange)) {
        updateMarkerColor();
    }
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

void GraphPanelWidget::addGraph(GraphPanelPlotGraph *pGraph,
                                const GraphPanelPlotGraphProperties &pGraphProperties)
{
    // Add the graph to our plot

    if (mPlot->addGraph(pGraph)) {
        emit graphAdded(this, pGraph, pGraphProperties);
    }
}

//==============================================================================

void GraphPanelWidget::removeGraphs(const GraphPanelPlotGraphs &pGraphs)
{
    // Remove the given graphs from our plot

    GraphPanelPlotGraphs graphs = GraphPanelPlotGraphs();

    for (auto graph : pGraphs) {
        if (mPlot->removeGraph(graph)) {
            graphs << graph;
        }
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

    mMarker->setStyleSheet(QString("QFrame {"
                                   "    color: %1;"
                                   "}").arg(mActive?
                                                isEnabled()?
                                                    Core::highlightColor().name():
                                                    Core::highlightColor(QPalette::Disabled).name(QColor::HexArgb):
                                                isEnabled()?
                                                    Core::windowColor().name():
                                                    Core::windowColor(QPalette::Disabled).name(QColor::HexArgb)));
}

//==============================================================================

void GraphPanelWidget::setActive(bool pActive, bool pForce)
{
    if ((pActive == mActive) && !pForce) {
        return;
    }

    // Set the graph panel's active state

    mActive = pActive;

    // Update the marker's colour

    updateMarkerColor();

    // Let people know if the graph panel has been activated or inactivated

    if (pActive) {
        emit activated(this);
    } else {
        emit inactivated(this);
    }
}

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
