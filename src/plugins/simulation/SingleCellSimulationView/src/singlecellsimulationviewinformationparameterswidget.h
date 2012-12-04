//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H

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

class SingleCellSimulationViewInformationParametersWidget : public QtPropertyBrowserSupport::QtPropertyBrowserWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationParametersWidget(QWidget *pParent = 0);

    void initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
