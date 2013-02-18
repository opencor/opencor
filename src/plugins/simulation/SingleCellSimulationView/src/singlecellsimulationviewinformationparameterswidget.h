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
                    CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime,
                    SingleCellSimulationViewSimulationData *pSimulationData);

    void cancelPropertyEditing();

private:
    QMap<QString, Core::PropertyEditorWidget *> mPropertyEditors;

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeModelParameter *> mModelParameters;

    QList<int> mColumnWidths;

    SingleCellSimulationViewSimulationData *mSimulationData;

    void populateModel(Core::PropertyEditorWidget *pPropertyEditor,
                       CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);

Q_SIGNALS:
void showHideParameterPlot(const QString &pFileName,
                           CellMLSupport::CellmlFileRuntimeModelParameter *pParameter,
                           const bool &pShowParameterPlot);
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...

public Q_SLOTS:
    void updateParameters();

private Q_SLOTS:
    void propertyEditorSectionResized(const int &pLogicalIndex,
                                      const int &pOldSize, const int &pNewSize);

    void propertyChanged(Core::Property *pProperty);

void emitShowHideParameterPlot(Core::Property *pProperty,
                               const bool &pShowParameterPlot);
//---GRY--- THE ABOVE IS TEMPORARY, JUST FOR OUR DEMO...
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
