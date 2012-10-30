//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewSimulationInformationWidget;
}

//==============================================================================

class QDesignerFormEditorInterface;
class QDesignerPropertyEditorInterface;

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewSimulationInformationWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewSimulationInformationWidget(QWidget *pParent);
    ~SingleCellSimulationViewSimulationInformationWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::SingleCellSimulationViewSimulationInformationWidget *mGui;

    QDesignerFormEditorInterface *mFormEditor;
    QDesignerPropertyEditorInterface *mPropertyEditor;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
