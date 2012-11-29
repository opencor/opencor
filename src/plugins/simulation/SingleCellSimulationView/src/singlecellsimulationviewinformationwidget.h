//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewInformationWidget;
}

//==============================================================================

class QtProperty;
class QtTreePropertyBrowser;

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

class SingleCellSimulationViewInformationWidget : public Core::Widget
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

private:
    Ui::SingleCellSimulationViewInformationWidget *mGui;

    QtTreePropertyBrowser *mPropertyEditor;

    QtProperty *mSimulationSection;
    QtProperty *mSolversSection;
    QtProperty *mTracesSection;
    QtProperty *mParametersSection;

    Core::CollapsibleWidget *mSimulationCollapsibleWidget;
    SingleCellSimulationViewInformationSimulationWidget *mSimulationWidget;

    Core::CollapsibleWidget *mSolversCollapsibleWidget;
    SingleCellSimulationViewInformationSolversWidget *mSolversWidget;

    Core::CollapsibleWidget *mTracesCollapsibleWidget;

    Core::CollapsibleWidget *mParametersCollapsibleWidget;
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
