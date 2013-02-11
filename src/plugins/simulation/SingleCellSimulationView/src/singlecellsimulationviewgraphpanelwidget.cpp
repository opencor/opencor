//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelwidget.h"

//==============================================================================

#include <QHBoxLayout>
#include <QMouseEvent>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

//==============================================================================

#include "ui_singlecellsimulationviewgraphpanelwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelWidget::SingleCellSimulationViewGraphPanelWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewGraphPanelWidget),
    mActive(false),
    mPlotTraces(QList<QwtPlotCurve *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create, customise and add an inactive marker to our layout

    static const int MarkerWidth = 3;

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    mGui->layout->addWidget(mMarker);

    // Create, customise and add a QwtPlot widget to our layout

    mPlot = new QwtPlot(this);

    mPlot->setAutoReplot(true);
    mPlot->setCanvasBackground(Qt::white);

    qobject_cast<QwtPlotCanvas *>(mPlot->canvas())->setFrameShape(QFrame::NoFrame);

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajorPen(Qt::gray, 0, Qt::DotLine);

    grid->attach(mPlot);

    mGui->layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

//==============================================================================

SingleCellSimulationViewGraphPanelWidget::~SingleCellSimulationViewGraphPanelWidget()
{
    // Delete some internal objects

    removeTraces();

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colour
    // used to highlight the active graph panel

    if (pEvent->type() == QEvent::PaletteChange)
        updateMarkerColor();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QWidget::mousePressEvent(pEvent);

    // Activate/inactivate the graph panel
    // Note: we do it through setActive() because we want the graph panel to let
    //       people know that our active state has changed...

    setActive(true);
}

//==============================================================================

QwtPlotCurve * SingleCellSimulationViewGraphPanelWidget::addTrace()
{
    // Create a new trace

    QwtPlotCurve *res = new QwtPlotCurve();

    // Customise it a bit

    res->setRenderHint(QwtPlotItem::RenderAntialiased);
    res->setPen(QPen(Qt::darkBlue));

    // Attach it to ourselves

    res->attach(mPlot);

    // Add it to our list of traces

    mPlotTraces << res;

    // Return it to the caller

    return res;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelWidget::removeTrace(QwtPlotCurve *pTrace,
                                                           const bool &pReplot)
{
    // Make sure that we have a trace

    if (!pTrace)
        return;

    // Detach and then delete the trace

    pTrace->detach();

    delete pTrace;

    // Stop tracking the trace

    mPlotTraces.removeOne(pTrace);

    // Replot ourselves, if needed

    if (pReplot)
        mPlot->replot();
}

//==============================================================================

void SingleCellSimulationViewGraphPanelWidget::removeTraces()
{
    // Remove any existing trace

    foreach (QwtPlotCurve *trace, mPlotTraces)
        removeTrace(trace, false);

    // Replot ourselves

    mPlot->replot();
}

//==============================================================================

QwtPlot * SingleCellSimulationViewGraphPanelWidget::plot()
{
    // Return the pointer to our plot widget

    return mPlot;
}

//==============================================================================

bool SingleCellSimulationViewGraphPanelWidget::isActive() const
{
    // Return whether the graph panel as active

    return mActive;
}

//==============================================================================

void SingleCellSimulationViewGraphPanelWidget::updateMarkerColor()
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

void SingleCellSimulationViewGraphPanelWidget::setActive(const bool &pActive)
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
