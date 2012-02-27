//==============================================================================
// Single cell simulation view
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

//==============================================================================

#include <QWidget>

//==============================================================================

class QwtPlot;
class QwtPlotCurve;

//==============================================================================

namespace Ui {
    class SingleCellSimulationView;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationView(QWidget *pParent = 0);
    ~SingleCellSimulationView();

    virtual void retranslateUi();

private:
    Ui::SingleCellSimulationView *mUi;

    QwtPlot *mSimulationView;

    QList<QwtPlotCurve *> mCurves;

    void resetCurves();
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
