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

namespace OpenCOR {

//==============================================================================

namespace Core {
    class CollapsibleWidget;
}   // namespace Core

//==============================================================================

namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationInformationWidget;

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

private:
    Ui::SingleCellSimulationViewInformationWidget *mGui;

    Core::CollapsibleWidget *mSimulationCollapsibleWidget;
    SingleCellSimulationViewSimulationInformationWidget *mSimulationInformationWidget;

    Core::CollapsibleWidget *mSolversCollapsibleWidget;

    Core::CollapsibleWidget *mTracesCollapsibleWidget;

    Core::CollapsibleWidget *mParametersCollapsibleWidget;

private Q_SLOTS:
    void currentEditor(QWidget *pNewEditor);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
