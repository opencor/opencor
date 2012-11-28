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
    mWidth(0),
    mValue(0.0)
{
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    int value = mValue*mWidth;

    if (value)
        painter.fillRect(0, 0, value, height(),
                         Core::CommonWidget::highlightColor());

    if (value != mWidth)
        painter.fillRect(value, 0, mWidth-value, height(),
                         Core::CommonWidget::windowColor());

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Keep track of our new width

    mWidth = pEvent->size().width();

    // Default handling of the event

    QWidget::resizeEvent(pEvent);
}

//==============================================================================

void SingleCellSimulationViewProgressBarWidget::setValue(const double &pValue)
{
    // Update both our value and ourselves, if needed

    double value = qMin(1.0, qMax(pValue, 0.0));

    if (value != mValue) {
        bool needUpdate = int(value*mWidth) != int(mValue*mWidth);

        mValue = value;

        if (needUpdate)
            update();
    }
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
