//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationInformationWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
