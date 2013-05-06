//==============================================================================
// Single cell view information parameters widget
//==============================================================================

#ifndef SINGLECELLVIEWINFORMATIONPARAMETERSWIDGET_H
#define SINGLECELLVIEWINFORMATIONPARAMETERSWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "cellmlfileruntime.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class Property;
    class PropertyEditorWidget;
}   // namespace Core

//==============================================================================

namespace CellMLSupport {
    class CellMLFileRuntime;
    class CellMLFileRuntimeCompiledModelParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulationData;

//==============================================================================

class SingleCellViewInformationParametersWidget : public QStackedWidget,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationParametersWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName,
                    CellMLSupport::CellMLFileRuntime *pRuntime,
                    SingleCellViewSimulationData *pSimulationData);
    void finalize(const QString &pFileName);

    void finishPropertyEditing();

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;

    QMap<Core::Property *, QExplicitlySharedDataPointer<CellMLSupport::CellMLFileRuntimeModelParameter> > mModelParameters;

    QList<int> mColumnWidths;

    SingleCellViewSimulationData *mSimulationData;

    void populateModel(Core::PropertyEditorWidget *pPropertyEditor,
                       CellMLSupport::CellMLFileRuntime *pRuntime);

    void updateModelParametersToolTips();

Q_SIGNALS:
    void showModelParameter(const QString &pFileName,
                            QExplicitlySharedDataPointer<CellMLSupport::CellMLFileRuntimeModelParameter> pParameter,
                            const bool &pShow);

public Q_SLOTS:
    void updateParameters();

private Q_SLOTS:
    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

    void propertyChanged(Core::Property *pProperty);

    void emitShowModelParameter(Core::Property *pProperty, const bool &pShow);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
