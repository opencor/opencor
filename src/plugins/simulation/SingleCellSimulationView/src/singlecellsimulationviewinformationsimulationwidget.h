//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSIMULATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSIMULATIONWIDGET_H

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

class SingleCellSimulationViewInformationSimulationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewInformationSimulationWidget();

    virtual void retranslateUi();

    void initialize(const QString &pFileName,
                    OpenCOR::CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    void finalize(const QString &pFileName);

    Core::Property * startingPointProperty() const;
    Core::Property * endingPointProperty() const;
    Core::Property * pointIntervalProperty() const;

    double startingPoint() const;
    double endingPoint() const;
    double pointInterval() const;

private:
    Core::Property *mStartingPointProperty;
    Core::Property *mEndingPointProperty;
    Core::Property *mPointIntervalProperty;

    QMap<QString, Core::PropertyEditorWidgetGuiState *> mGuiStates;
    Core::PropertyEditorWidgetGuiState *mDefaultGuiState;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
