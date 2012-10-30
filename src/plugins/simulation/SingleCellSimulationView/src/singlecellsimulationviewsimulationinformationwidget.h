//==============================================================================
// Single cell simulation view simulation information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWSIMULATIONINFORMATIONWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

class QLabel;
class QLineEdit;

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewSimulationInformationWidget;
}

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

    QLabel *mStartingPointLabel;
    QLineEdit *mStartingPointField;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
