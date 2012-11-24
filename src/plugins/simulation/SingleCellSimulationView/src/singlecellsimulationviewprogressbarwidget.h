//==============================================================================
// Single cell simulation view progress bar widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWPROGRESSBARWIDGET_H
#define SINGLECELLSIMULATIONVIEWPROGRESSBARWIDGET_H

//==============================================================================

#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewProgressBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewProgressBarWidget(QWidget *pParent = 0);

    void setValue(const double &pValue);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void paintEvent(QPaintEvent *pEvent);

private:
    double mValue;

    QColor mBackgroundColor;
    QColor mForegroundColor;

    void updateColors();
};

//==============================================================================

}   // namespace Core
}   // namespace SingleCellSimulationView

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
