//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#include "singlecellsimulationviewgraphpanel.h"

//==============================================================================

#include <QApplication>
#include <QHBoxLayout>
#include <QPaintEvent>

//==============================================================================

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"

//==============================================================================

#include "ui_singlecellsimulationviewgraphpanel.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanel::SingleCellSimulationViewGraphPanel(QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewGraphPanel),
    mActive(false),
    mPlotCurves(QList<QwtPlotCurve *>())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create, customise and add a marker to our horizontal layout

    static const int MarkerWidth = 3;

    mMarker = new QFrame(this);

    mMarker->setFrameShape(QFrame::VLine);
    mMarker->setLineWidth(MarkerWidth);
    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

    mGui->layout->addWidget(mMarker);

    // Create, customise and add a QwtPlot widget to our horizontal layout

    mPlot = new QwtPlot(this);

    mPlot->setCanvasBackground(Qt::white);
    mPlot->setCanvasLineWidth(0);
    mPlot->setFrameStyle(QFrame::NoFrame);

    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));

    grid->attach(mPlot);

    mGui->layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

//==============================================================================

SingleCellSimulationViewGraphPanel::~SingleCellSimulationViewGraphPanel()
{
    // Delete some internal objects

    resetCurves();

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void SingleCellSimulationViewGraphPanel::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update the colour
    // used to highlight the active graph panel

    if (pEvent->type() == QEvent::PaletteChange)
        setMarkerColor();
}

//==============================================================================

void SingleCellSimulationViewGraphPanel::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QWidget::mousePressEvent(pEvent);

    // Activate/inactivate the graph panel
    // Note: we do it through setActive() because we want the graph panel to let
    //       people know that our active state has changed...

    setActive(true);
}

//==============================================================================

QwtPlotCurve * SingleCellSimulationViewGraphPanel::addCurve()
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

void SingleCellSimulationViewGraphPanel::resetCurves()
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

QwtPlot * SingleCellSimulationViewGraphPanel::plot()
{
    // Return the pointer to our plot widget

    return mPlot;
}

//==============================================================================

bool SingleCellSimulationViewGraphPanel::isActive() const
{
    // Return whether the graph panel as active

    return mActive;
}

//==============================================================================

void SingleCellSimulationViewGraphPanel::setMarkerColor()
{
    // Set the marker's colour based on whether the graph panel is active or not

    QPalette markerPalette = qApp->palette();

    markerPalette.setColor(QPalette::WindowText, mActive?
                                                    markerPalette.color(QPalette::Highlight):
                                                    markerPalette.color(QPalette::Window));

    mMarker->setPalette(markerPalette);
}

//==============================================================================

void SingleCellSimulationViewGraphPanel::setActive(const bool &pActive)
{
    if (pActive == mActive)
        return;

    // Set the graph panel's active state

    mActive = pActive;

    // Update the marker's colour

    setMarkerColor();

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
