//==============================================================================
// Single cell simulation view information simulation widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONSIMULATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONSIMULATIONWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationSimulationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationSimulationWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    void setUnit(const QString &pUnit);

    double startingPoint() const;
    void setStartingPoint(const double &pValue);

    double endingPoint() const;
    void setEndingPoint(const double &pValue);

    double pointInterval() const;
    void setPointInterval(const double &pValue);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
