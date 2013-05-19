//==============================================================================
// Single cell view information solvers widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H
#define SINGLECELLVIEWINFORMATIONSOLVERSWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellMLFileRuntime;
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

    void setupBuiltInSolvers();

    void initialize(const QString &pFileName,
                    CellMLSupport::CellMLFileRuntime *pRuntime,
                    SingleCellViewSimulationData *pSimulationData);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    QStringList solvers() const;

    SingleCellViewInformationSolversWidgetData * solverData() const;

private:
    SingleCellViewInformationSolversWidgetData *mSolverData;

    QMap<QString, Core::PropertyEditorWidgetGuiState *> mGuiStates;
    Core::PropertyEditorWidgetGuiState *mDefaultGuiState;

    QMap<Core::Property *, Descriptions> mDescriptions;

    void resetAllGuiStates();

    bool doSolverChanged(SingleCellViewInformationSolversWidgetData *pSolverData,
                         const QString &pSolverName,
                         const bool &pForceHandling = false);

    void setPropertiesUnit(SingleCellViewInformationSolversWidgetData *pSolverData,
                           const QString &pVoiUnit);

    void updatePropertiesToolTip();

private Q_SLOTS:
    void solverChanged(const QString &pValue);

    void updatePropertyToolTip(Core::Property *pProperty);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
