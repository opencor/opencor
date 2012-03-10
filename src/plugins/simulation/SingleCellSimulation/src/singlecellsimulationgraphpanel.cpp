//==============================================================================
// Single cell simulation graph panel widget
//==============================================================================

#include "singlecellsimulationgraphpanel.h"

//==============================================================================

#include <QHBoxLayout>
#include <QPaintEvent>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationGraphPanel::SingleCellSimulationGraphPanel(QWidget *pParent) :
    QWidget(pParent),
    mActive(false),
    mPlotCurves(QList<QwtPlotCurve *>())
{
    // Create, customise and set a horizontal layout where we are going to put
    // our marker and QwtPlot widget

    QHBoxLayout *horizontalLayout= new QHBoxLayout(this);

    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(horizontalLayout);

    // Create, customise and add a marker to our horizontal layout

    static const int MarkerWidth = 3;

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    horizontalLayout->addWidget(mMarker);

    // Create, customise and add a QwtPlot widget to our horizontal layout

    mPlot = new QwtPlot(this);

    mPlot->setCanvasBackground(Qt::white);
    mPlot->setCanvasLineWidth(0);
    mPlot->setFrameStyle(QFrame::NoFrame);

    QwtPlotGrid *grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(mPlot);

    horizontalLayout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

//==============================================================================

SingleCellSimulationGraphPanel::~SingleCellSimulationGraphPanel()
{
    // Delete some internal objects

    resetCurves();
}

//==============================================================================

void SingleCellSimulationGraphPanel::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QWidget::mousePressEvent(pEvent);

    // Activate/inactivate the graph panel
    // Note: we do it through setActive() because we want the graph panel to let
    //       people know that our active state has changed...

    setActive(true);
}

//==============================================================================

QwtPlotCurve * SingleCellSimulationGraphPanel::addCurve()
{
    // Create a new curve

    QwtPlotCurve *res = new QwtPlotCurve();

    // Customise it a bit

    res->setRenderHint(QwtPlotItem::RenderAntialiased);
    res->setPen(QPen(Qt::darkBlue));

    // Attach it to ourselves

    res->attach(mPlot);

    // Add it to our list of curves

    mPlotCurves.append(res);

    // Return it to the caller

    return res;
}

//==============================================================================

void SingleCellSimulationGraphPanel::resetCurves()
{
    // Remove any existing curve

    foreach (QwtPlotCurve *curve, mPlotCurves) {
        curve->detach();

        delete curve;
    }

    mPlotCurves.clear();

    // Refresh the graph panel

    mPlot->replot();
}

//==============================================================================

QwtPlot * SingleCellSimulationGraphPanel::plot()
{
    // Return the pointer to our plot widget

    return mPlot;
}

//==============================================================================

bool SingleCellSimulationGraphPanel::isActive() const
{
    // Return whether the graph panel as active

    return mActive;
}

//==============================================================================

void SingleCellSimulationGraphPanel::setActive(const bool &pActive)
{
    if (pActive == mActive)
        return;

    // Set the graph panel's active state

    mActive = pActive;

    // Update the marker's colour

    QPalette markerPalette = mMarker->palette();

    markerPalette.setColor(QPalette::WindowText, pActive?
                                                    markerPalette.color(QPalette::Highlight):
                                                    markerPalette.color(QPalette::Window));

    mMarker->setPalette(markerPalette);

    // Let people know if the graph panel has been activated or inactivated

    if (pActive)
        emit activated(this);
    else
        emit inactivated(this);
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
