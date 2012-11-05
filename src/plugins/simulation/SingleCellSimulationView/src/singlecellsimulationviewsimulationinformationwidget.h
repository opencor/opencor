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
