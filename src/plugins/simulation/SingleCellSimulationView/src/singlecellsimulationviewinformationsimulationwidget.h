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

struct SingleCellSimulationViewInformationSimulationWidgetData
{
    explicit SingleCellSimulationViewInformationSimulationWidgetData();

    double startingPoint;
    double endingPoint;
    double pointInterval;

    QModelIndex currentIndex;
};

//==============================================================================

class SingleCellSimulationViewInformationSimulationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    void initialize(const QString &pFileName,
                    OpenCOR::CellMLSupport::CellmlFileRuntime *pCellmlFileRuntime);
    void finalize(const QString &pFileName);

    double startingPoint() const;
    double endingPoint() const;
    double pointInterval() const;

private:
    Core::Property mStartingPointProperty;
    Core::Property mEndingPointProperty;
    Core::Property mPointIntervalProperty;

    QMap<QString, SingleCellSimulationViewInformationSimulationWidgetData> mData;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
