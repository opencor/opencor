//==============================================================================
// Single cell simulation view information solvers widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    void initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                    const SolverInterfaces &pSolverInterfaces);

    bool needOdeSolver() const;
    bool needNlaSolver() const;

    QStringList odeOrDaeSolvers() const;
    QStringList nlaSolvers() const;

private:
    bool mNeedOdeSolver;
    bool mNeedNlaSolver;

    Core::Property mOdeOrDaeSolversProperty;
    Core::Property mNlaSolversProperty;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
