//==============================================================================
// Single cell simulation view progress bar widget
//==============================================================================

#include "commonwidget.h"
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
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    int value = mValue*width();

    if (value)
        painter.fillRect(0, 0, value, height(),
                         Core::CommonWidget::highlightColor());

    if (value != width())
        painter.fillRect(value, 0, width()-value, height(),
                         Core::CommonWidget::windowColor());

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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
