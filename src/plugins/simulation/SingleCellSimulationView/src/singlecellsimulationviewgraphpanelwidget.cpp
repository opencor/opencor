//==============================================================================
// Single cell simulation view graph panel widget
//==============================================================================

#include "singlecellsimulationviewgraphpanelplotwidget.h"
#include "singlecellsimulationviewgraphpanelwidget.h"

//==============================================================================

#include <QHBoxLayout>
#include <QMouseEvent>

//==============================================================================

#include "qwt_plot_curve.h"

//==============================================================================

#include "ui_singlecellsimulationviewgraphpanelwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewGraphPanelWidget::SingleCellSimulationViewGraphPanelWidget(QWidget *pParent) :
    Widget(pParent),
    mGui(new Ui::SingleCellSimulationViewGraphPanelWidget),
    mActive(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create, customise and add an inactive marker to our layout

//    enum {
//        MarkerWidth = 3
//    };

//    mMarker = new QFrame(this);

//    mMarker->setFrameShape(QFrame::VLine);
//    mMarker->setLineWidth(MarkerWidth);
//    mMarker->setMinimumWidth(MarkerWidth);

    setActive(false);

//    mGui->layout->addWidget(mMarker);
//---GRY--- THIS IS TEMPORARY, I.E. WHILE WE SUPPORT ONLY ONE GRAPH PANEL...

    // Create and add a plot widget to our layout

    mPlot = new SingleCellSimulationViewGraphPanelPlotWidget(this);

    mGui->layout->addWidget(mPlot);

    // Allow the graph panel to be of any vertical size

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

//==============================================================================

SingleCellSimulationViewGraphPanelWidget::~SingleCellSimulationViewGraphPanelWidget()
{
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

SingleCellSimulationViewGraphPanelPlotWidget * SingleCellSimulationViewGraphPanelWidget::plot()
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

//    mMarker->setPalette(newPalette);
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
