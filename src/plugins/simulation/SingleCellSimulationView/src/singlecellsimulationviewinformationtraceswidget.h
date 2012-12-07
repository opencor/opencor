//==============================================================================
// Single cell simulation view information traces widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONTRACESWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONTRACESWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationTracesWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationTracesWidget(QWidget *pParent = 0);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
