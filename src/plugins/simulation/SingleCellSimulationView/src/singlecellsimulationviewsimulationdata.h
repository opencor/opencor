//==============================================================================
// Single cell simulation view simulation data
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONDATA_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONDATA_H

//==============================================================================

class QwtSlider;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationSimulationWidget;

//==============================================================================

class SingleCellSimulationViewSimulationData
{
public:
    explicit SingleCellSimulationViewSimulationData();

    void updateFromGui(QwtSlider *pSlider,
                       SingleCellSimulationViewInformationSimulationWidget *pSimulationSettings);
    void updateGui(QwtSlider *pSlider,
                   SingleCellSimulationViewInformationSimulationWidget *pSimulationSettings);

    int delay() const;
    void setDelay(const int &pDelay);

    double startingPoint() const;
    double endingPoint() const;
    double pointInterval() const;

private:
    int mDelay;

    double mStartingPoint;
    double mEndingPoint;
    double mPointInterval;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
