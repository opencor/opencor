//==============================================================================
// Single cell simulation graph panels widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONGRAPHPANELS_H
#define SINGLECELLSIMULATIONGRAPHPANELS_H

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

class SingleCellSimulationGraphPanels : public QSplitter
{
    Q_OBJECT

public:
    explicit SingleCellSimulationGraphPanels(QWidget *pParent = 0);
};

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
