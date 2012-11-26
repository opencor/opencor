//==============================================================================
// Single cell simulation view simulation data
//==============================================================================

#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulationdata.h"

//==============================================================================

#include "qwt_slider.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

SingleCellSimulationViewSimulationData::SingleCellSimulationViewSimulationData() :
    mDelay(0),
    mStartingPoint(0.0),
    mEndingPoint(1000.0),
    mPointInterval(1.0)
{
}

//==============================================================================

void SingleCellSimulationViewSimulationData::updateFromGui(QwtSlider *pSlider,
                                                           SingleCellSimulationViewInformationWidget *pSettings)
{
    // Update our delay from our slider

    mDelay = pSlider->value();

    // Update our settings from our simulation information widget

    mStartingPoint = pSettings->simulationWidget()->startingPoint();
    mEndingPoint   = pSettings->simulationWidget()->endingPoint();
    mPointInterval = pSettings->simulationWidget()->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::updateGui(QwtSlider *pSlider,
                                                       SingleCellSimulationViewInformationWidget *pSettings)
{
    // Update our slider using our delay

    pSlider->setValue(mDelay);

    // Update our simulation information widget using our settings

    pSettings->simulationWidget()->setStartingPoint(mStartingPoint);
    pSettings->simulationWidget()->setEndingPoint(mEndingPoint);
    pSettings->simulationWidget()->setPointInterval(mPointInterval);
}

//==============================================================================

int SingleCellSimulationViewSimulationData::delay() const
{
    // Return our delay

    return mDelay;
}

//==============================================================================

void SingleCellSimulationViewSimulationData::setDelay(const int &pDelay)
{
    // Set our delay

    mDelay = pDelay;
}

//==============================================================================

double SingleCellSimulationViewSimulationData::startingPoint() const
{
    // Return our starting point

    return mStartingPoint;
}

//==============================================================================

double SingleCellSimulationViewSimulationData::endingPoint() const
{
    // Return our ending point

    return mEndingPoint;
}

//==============================================================================

double SingleCellSimulationViewSimulationData::pointInterval() const
{
    // Return our point interval

    return mPointInterval;
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
