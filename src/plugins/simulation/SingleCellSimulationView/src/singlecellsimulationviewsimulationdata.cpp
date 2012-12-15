//==============================================================================
// Single cell simulation view simulation data
//==============================================================================

#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulationdata.h"
#include "singlecellsimulationviewwidget.h"

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

void SingleCellSimulationViewSimulationData::updateFromGui(SingleCellSimulationViewWidget *pGui)
{
    // Update our delay from our GUI's delay widget

    mDelay = pGui->delayValue();

    // Update some of our data from our GUI's simulation widget

    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = pGui->contentsWidget()->informationWidget()->simulationWidget();

    mStartingPoint = simulationWidget->startingPoint();
    mEndingPoint   = simulationWidget->endingPoint();
    mPointInterval = simulationWidget->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::updateGui(SingleCellSimulationViewWidget *pGui)
{
    // Update our GUI's delay widget using our delay

    pGui->setDelayValue(mDelay);

    // Update our GUI's simulation widget using some of our data

    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = pGui->contentsWidget()->informationWidget()->simulationWidget();

    simulationWidget->setStartingPoint(mStartingPoint);
    simulationWidget->setEndingPoint(mEndingPoint);
    simulationWidget->setPointInterval(mPointInterval);
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
