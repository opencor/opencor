//==============================================================================
// Single cell simulation view progress bar widget
//==============================================================================

#include "singlecellsimulationviewprogressbarwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewProgressBarWidget::SingleCellSimulationViewProgressBarWidget(QWidget *pParent) :
    QWidget(pParent),
    mValue(0.0)
{
    // Initialise our colours

    updateColors();
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QWidget::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update both our
    // colours and ourselves

    if (pEvent->type() == QEvent::PaletteChange) {
        updateColors();

        update();
    }
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    int value = mValue*width();

    if (value)
        painter.fillRect(0, 0, value, height(), mForegroundColor);

    if (value != width())
        painter.fillRect(value, 0, width()-value, height(), mBackgroundColor);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::setValue(const double &pValue)
{
    // Update both our value and ourselves, if needed

    double value = qMin(1.0, qMax(pValue, 0.0));

    if (value != mValue) {
        mValue = value;

        update();
    }
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::updateColors()
{
    // Update both our background and foreground colours

    QPalette widgetPalette = palette();

    mBackgroundColor = widgetPalette.color(QPalette::Window);
    mForegroundColor = widgetPalette.color(QPalette::Highlight);
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
