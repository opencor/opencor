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

class SingleCellSimulationViewSimulationData;

//==============================================================================

class SingleCellSimulationViewInformationSolversWidgetData
{
public:
    explicit SingleCellSimulationViewInformationSolversWidgetData(Core::Property *pSolversProperty,
                                                                  Core::Property *pSolversListProperty,
                                                                  const QMap<QString, Core::Properties> &pSolversProperties);

    Core::Property * solversProperty() const;
    Core::Property * solversListProperty() const;

    QMap<QString, Core::Properties> solversProperties() const;

private:
    Core::Property *mSolversProperty;
    Core::Property *mSolversListProperty;

    QMap<QString, Core::Properties> mSolversProperties;
};

//==============================================================================

class SingleCellSimulationViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSolversWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewInformationSolversWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                    SingleCellSimulationViewSimulationData *pSimulationData);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

    SingleCellSimulationViewInformationSolversWidgetData * odeSolverData() const;
    SingleCellSimulationViewInformationSolversWidgetData * daeSolverData() const;
    SingleCellSimulationViewInformationSolversWidgetData * nlaSolverData() const;

private:
    SingleCellSimulationViewInformationSolversWidgetData *mOdeSolverData;
    SingleCellSimulationViewInformationSolversWidgetData *mDaeSolverData;
    SingleCellSimulationViewInformationSolversWidgetData *mNlaSolverData;

    QMap<QString, Core::PropertyEditorWidgetGuiState *> mGuiStates;
    Core::PropertyEditorWidgetGuiState *mDefaultGuiState;

    void resetAllGuiStates();

    SingleCellSimulationViewInformationSolversWidgetData * addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                               const Solver::Type &pSolverType);

    bool doListPropertyChanged(SingleCellSimulationViewInformationSolversWidgetData *pSolverData,
                               const QString &pSolverName,
                               const bool &pForceHandling = false);

    void setPropertiesUnit(SingleCellSimulationViewInformationSolversWidgetData *pSolverData,
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
