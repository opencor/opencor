//==============================================================================
// Single cell view information solvers widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H

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

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulationData;

//==============================================================================

class SingleCellViewInformationSolversWidgetData
{
public:
    explicit SingleCellViewInformationSolversWidgetData(Core::Property *pSolversProperty,
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

class SingleCellViewInformationSolversWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationSolversWidget(QWidget *pParent = 0);
    ~SingleCellViewInformationSolversWidget();

    virtual void retranslateUi();

    void setSolverInterfaces(const SolverInterfaces &pSolverInterfaces);

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellViewSimulationData *pSimulationData);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    QStringList odeSolvers() const;
    QStringList daeSolvers() const;
    QStringList nlaSolvers() const;

    SingleCellViewInformationSolversWidgetData * odeSolverData() const;
    SingleCellViewInformationSolversWidgetData * daeSolverData() const;
    SingleCellViewInformationSolversWidgetData * nlaSolverData() const;

private:
    SingleCellViewInformationSolversWidgetData *mOdeSolverData;
    SingleCellViewInformationSolversWidgetData *mDaeSolverData;
    SingleCellViewInformationSolversWidgetData *mNlaSolverData;

    QMap<QString, Core::PropertyEditorWidgetGuiState *> mGuiStates;
    Core::PropertyEditorWidgetGuiState *mDefaultGuiState;

    QMap<Core::Property *, Descriptions> mDescriptions;

    void resetAllGuiStates();

    SingleCellViewInformationSolversWidgetData * addSolverProperties(const SolverInterfaces &pSolverInterfaces,
                                                                     const Solver::Type &pSolverType);

    bool doSolverChanged(SingleCellViewInformationSolversWidgetData *pSolverData,
                         const QString &pSolverName,
                         const bool &pForceHandling = false);

    void setPropertiesUnit(SingleCellViewInformationSolversWidgetData *pSolverData,
                           const QString &pVoiUnit);

private Q_SLOTS:
    void solverChanged(const QString &pValue);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
