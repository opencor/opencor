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

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    void initialize(CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                    const SolverInterfaces &pSolverInterfaces);

    bool needOdeSolver() const;
    bool needDaeSolver() const;
    bool needNlaSolver() const;

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

private:
    bool mNeedOdeSolver;
    bool mNeedDaeSolver;
    bool mNeedNlaSolver;

    Core::Property mOdeSolversProperty;
    Core::Property mDaeSolversProperty;
    Core::Property mNlaSolversProperty;

    Core::Property addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                       const Solver::Type &pSolverType);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
