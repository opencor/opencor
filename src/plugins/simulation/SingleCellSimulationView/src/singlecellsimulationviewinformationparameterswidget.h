//==============================================================================
// Single cell simulation view information parameters widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONPARAMETERSWIDGET_H

//==============================================================================

#include "commonwidget.h"

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
    class CellmlFileRuntime;
    class CellmlFileRuntimeModelParameter;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationData;

//==============================================================================

class SingleCellSimulationViewInformationParametersWidget : public QStackedWidget,
                                                            public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationParametersWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName,
                    CellMLSupport::CellmlFileRuntime *pRuntime,
                    SingleCellSimulationViewSimulationData *pSimulationData);
    void finalize(const QString &pFileName);

    void finishPropertyEditing();

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeModelParameter *> mModelParameters;

    QList<int> mColumnWidths;

    SingleCellSimulationViewSimulationData *mSimulationData;

    void populateModel(Core::PropertyEditorWidget *pPropertyEditor,
                       CellMLSupport::CellmlFileRuntime *pRuntime);

Q_SIGNALS:
    void showModelParameter(const QString &pFileName,
                            CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
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

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
