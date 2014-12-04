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
// Single cell view graph panel widget
//==============================================================================

#include "coreguiutils.h"
#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewgraphpanelwidget.h"

//==============================================================================

#include <QHBoxLayout>
#include <QMouseEvent>

//==============================================================================

#include "qwt_plot_curve.h"

//==============================================================================

#include "ui_singlecellviewgraphpanelwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewGraphPanelWidget::SingleCellViewGraphPanelWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellViewGraphPanelWidget),
    mActive(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create, customise and add an inactive marker to our layout

    enum {
        MarkerWidth = 3
    };

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    mGui->layout->addWidget(mMarker);

    // Create and add a plot widget to our layout

    mPlot = new SingleCellViewGraphPanelPlotWidget(this);

    mGui->layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

//==============================================================================

SingleCellViewGraphPanelWidget::~SingleCellViewGraphPanelWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellViewGraphPanelWidget::retranslateUi()
{
    // Retranslate our plot

    mPlot->retranslateUi();
}

//==============================================================================

void SingleCellViewGraphPanelWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colour
    // used to highlight the active graph panel

    if (pEvent->type() == QEvent::PaletteChange)
        updateMarkerColor();
}

//==============================================================================

void SingleCellViewGraphPanelWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QWidget::mousePressEvent(pEvent);

    // Activate/inactivate the graph panel
    // Note: we do it through setActive() because we want the graph panel to let
    //       people know that our active state has changed...

    setActive(true);
}

//==============================================================================

SingleCellViewGraphPanelPlotWidget * SingleCellViewGraphPanelWidget::plot() const
{
    // Return our plot

    return mPlot;
}

//==============================================================================

QList<SingleCellViewGraphPanelPlotGraph *> SingleCellViewGraphPanelWidget::graphs() const
{
    // Return all our plot's graphs

    return mPlot->graphs();
}

//==============================================================================

void SingleCellViewGraphPanelWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Add the graph to our plot

    if (mPlot->addGraph(pGraph))
        emit graphAdded(mPlot, pGraph);
}

//==============================================================================

void SingleCellViewGraphPanelWidget::removeGraphs(const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs)
{
    // Remove the graphs from our plot

    QList<SingleCellViewGraphPanelPlotGraph *> graphs = QList<SingleCellViewGraphPanelPlotGraph *>();

    foreach (SingleCellViewGraphPanelPlotGraph *graph, pGraphs)
        if (mPlot->removeGraph(graph))
            graphs << graph;

    emit graphsRemoved(mPlot, graphs);
}

//==============================================================================

bool SingleCellViewGraphPanelWidget::isActive() const
{
    // Return whether the graph panel is active

    return mActive;
}

//==============================================================================

void SingleCellViewGraphPanelWidget::updateMarkerColor()
{
    // Update the marker's colour based on whether the graph panel is active

    QPalette newPalette = palette();

    newPalette.setColor(QPalette::WindowText,
                        mActive?
                            Core::highlightColor():
                            Core::windowColor());

    mMarker->setPalette(newPalette);
}

//==============================================================================

void SingleCellViewGraphPanelWidget::setActive(const bool &pActive)
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

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
