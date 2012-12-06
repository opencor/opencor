//==============================================================================
// Single cell simulation view information traces widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONTRACESWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONTRACESWIDGET_H

//==============================================================================

#include "qtpropertybrowserwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationTracesWidget : public QtPropertyBrowserSupport::QtPropertyBrowserWidget
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
