//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationInformationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent = 0);

    virtual void retranslateUi();

    void setUnit(const QString &pUnit);

    double startingPoint() const;
    void setStartingPoint(const double &pValue);

    double endingPoint() const;
    void setEndingPoint(const double &pValue);

    double pointInterval() const;
    void setPointInterval(const double &pValue);

private:
    QStandardItemModel *mModel;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
