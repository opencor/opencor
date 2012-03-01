//==============================================================================
// Single cell simulation view
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEW_H
#define SINGLECELLSIMULATIONVIEW_H

//==============================================================================

#include <QWidget>

//==============================================================================

#include "qwt_plot.h"

//==============================================================================

class QProgressBar;
class QSplitter;

//==============================================================================

class QwtPlotCurve;

//==============================================================================

namespace Ui {
    class SingleCellSimulationView;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class GraphPanel : public QwtPlot
{
public:
    explicit GraphPanel(QWidget *pParent = 0);
    ~GraphPanel();

    void addCurve(QwtPlotCurve *pCurve, const bool &pRefresh = true);
    void resetCurves();

private:
    QList<QwtPlotCurve *> mCurves;
};

//==============================================================================

class SingleCellSimulationView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationView(QWidget *pParent = 0);
    ~SingleCellSimulationView();

    virtual void retranslateUi();

    void updateWith(const QString &pFileName);

private:
    Ui::SingleCellSimulationView *mUi;

    QSplitter *mVerticalSplitter;
    QProgressBar *mProgressBar;

    GraphPanel *mGraphPanel;
    QWidget *mSimulationOutput;

    GraphPanel * addGraphPanel();
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
