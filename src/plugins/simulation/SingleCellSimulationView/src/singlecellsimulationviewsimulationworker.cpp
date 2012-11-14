//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#include "singlecellsimulationviewsimulationworker.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

void SingleCellSimulationViewSimulationWorker::run()
{
    qDebug("Running the simulation...");

    emit finished();
}

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
