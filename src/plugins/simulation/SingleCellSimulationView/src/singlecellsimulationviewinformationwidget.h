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

namespace QtPropertyBrowserSupport {
    class QtPropertyBrowserWidget;
}   // namespace QtPropertyBrowserSupport

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationParametersWidget;
class SingleCellSimulationViewInformationSimulationWidget;
class SingleCellSimulationViewInformationSolversWidget;
class SingleCellSimulationViewInformationTracesWidget;

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
    SingleCellSimulationViewInformationTracesWidget * tracesWidget();
    SingleCellSimulationViewInformationParametersWidget * parametersWidget();

private:
    Ui::SingleCellSimulationViewInformationWidget *mGui;

    QtPropertyBrowserSupport::QtPropertyBrowserWidget *mPropertyEditor;

    Core::CollapsibleWidget *mCollapsibleWidget;

    SingleCellSimulationViewInformationSimulationWidget *mSimulationWidget;
    SingleCellSimulationViewInformationSolversWidget *mSolversWidget;
    SingleCellSimulationViewInformationTracesWidget *mTracesWidget;
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
