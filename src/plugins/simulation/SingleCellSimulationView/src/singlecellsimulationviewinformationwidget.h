//==============================================================================
// Single cell simulation view information widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H
#define SINGLECELLSIMULATIONVIEWINFORMATIONWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewInformationWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewInformationWidget : public QSplitter,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewInformationWidget(QWidget *pParent);
    ~SingleCellSimulationViewInformationWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::SingleCellSimulationViewInformationWidget *mGui;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
