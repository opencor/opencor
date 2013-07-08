//==============================================================================
// Single cell view graph panel widget
//==============================================================================

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

QList<SingleCellViewGraphPanelPlotGraph *> SingleCellViewGraphPanelWidget::graphs() const
{
    // Return all our plot's graphs

    return mPlot->graphs();
}

//==============================================================================

void SingleCellViewGraphPanelWidget::addGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Add the graph to our plot

    mPlot->addGraph(pGraph);
}

//==============================================================================

void SingleCellViewGraphPanelWidget::removeGraph(SingleCellViewGraphPanelPlotGraph *pGraph)
{
    // Remove the graph from our plot

    mPlot->removeGraph(pGraph);
}

//==============================================================================

bool SingleCellViewGraphPanelWidget::isActive() const
{
    // Return whether the graph panel as active

    return mActive;
}

//==============================================================================

void SingleCellViewGraphPanelWidget::updateMarkerColor()
{
    // Update the marker's colour based on whether the graph panel is active

    QPalette newPalette = palette();

    newPalette.setColor(QPalette::WindowText,
                        mActive?
                            CommonWidget::highlightColor():
                            CommonWidget::windowColor());

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
