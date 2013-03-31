//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QScrollArea>

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewInformationWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class CollapsibleWidget;
}   // namespace Core

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationParametersWidget;
class SingleCellSimulationViewInformationSimulationWidget;
class SingleCellSimulationViewInformationSolversWidget;

//==============================================================================

class SingleCellSimulationViewInformationWidget : public QScrollArea,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewInformationWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellSimulationViewInformationSimulationWidget * simulationWidget();
    SingleCellSimulationViewInformationSolversWidget * solversWidget();
    SingleCellSimulationViewInformationParametersWidget * parametersWidget();

    void cancelEditing();

private:
    Ui::SingleCellSimulationViewInformationWidget *mGui;

    Core::CollapsibleWidget *mCollapsibleWidget;

    SingleCellSimulationViewInformationSimulationWidget *mSimulationWidget;
    SingleCellSimulationViewInformationSolversWidget *mSolversWidget;
    SingleCellSimulationViewInformationParametersWidget *mParametersWidget;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
