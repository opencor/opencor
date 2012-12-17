//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H

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

class SingleCellSimulationViewInformationParametersWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationParametersWidget(QWidget *pParent = 0);

    void initialize(const QString &pFileName,
                    OpenCOR::CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    void finalize(const QString &pFileName);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
