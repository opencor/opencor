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

struct SingleCellSimulationViewInformationSolversWidgetData
{
    bool needSolver;

    Core::Property solversProperty;
    Core::Property solversListProperty;

    QMap<QString, Core::Properties> solversProperties;

    explicit SingleCellSimulationViewInformationSolversWidgetData();
};

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    void finalize(const QString &pFileName);

    bool needOdeSolver() const;
    bool needDaeSolver() const;
    bool needNlaSolver() const;

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

private:
    SingleCellSimulationViewInformationSolversWidgetData mOdeSolverData;
    SingleCellSimulationViewInformationSolversWidgetData mDaeSolverData;
    SingleCellSimulationViewInformationSolversWidgetData mNlaSolverData;

    void addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                             const Solver::Type &pSolverType,
                             SingleCellSimulationViewInformationSolversWidgetData &pSolverData);

    bool doListPropertyChanged(const SingleCellSimulationViewInformationSolversWidgetData &pSolverData,
                               const QString &pSolverName,
                               const bool &pForceHandling = false);

    void setPropertiesUnit(const SingleCellSimulationViewInformationSolversWidgetData &pSolverData,
                           const QString &pVoiUnit);

private Q_SLOTS:
    void listPropertyChanged(const QString &pValue);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
