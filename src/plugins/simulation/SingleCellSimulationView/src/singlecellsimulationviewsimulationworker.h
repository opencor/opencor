//==============================================================================
// Single cell simulation view simulation worker
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONWORKER_H

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationWorker : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void finished();

private Q_SLOTS:
    void run();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
