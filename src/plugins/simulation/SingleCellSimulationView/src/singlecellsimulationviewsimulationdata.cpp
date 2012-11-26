//==============================================================================
// Single cell simulation view simulation data
//==============================================================================

#include "singlecellsimulationviewcontentswidget.h"
#include "singlecellsimulationviewinformationsimulationwidget.h"
#include "singlecellsimulationviewinformationwidget.h"
#include "singlecellsimulationviewsimulationdata.h"
#include "singlecellsimulationviewwidget.h"

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

void SingleCellSimulationViewSimulationData::updateFromGui(SingleCellSimulationViewWidget *pGui)
{
    // Update our delay from our slider

    mDelay = pGui->delayWidget()->value();

    // Update our settings from our simulation information widget

    SingleCellSimulationViewInformationSimulationWidget *simulationWidget = pGui->contentsWidget()->informationWidget()->simulationWidget();

    mStartingPoint = simulationWidget->startingPoint();
    mEndingPoint   = simulationWidget->endingPoint();
    mPointInterval = simulationWidget->pointInterval();
}

//==============================================================================

void SingleCellSimulationViewSimulationData::updateGui(SingleCellSimulationViewWidget *pGui)
{
    // Update our slider using our delay

    pGui->delayWidget()->setValue(mDelay);

    // Update our simulation information widget using our settings

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
