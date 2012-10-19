//==============================================================================
// Single cell simulation view contents widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWCONTENTSWIDGET_H
#define SINGLECELLSIMULATIONVIEWCONTENTSWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewContentsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelsWidget;

//==============================================================================

class SingleCellSimulationViewContentsWidget : public QSplitter,
                                               public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewContentsWidget(QWidget *pParent);
    ~SingleCellSimulationViewContentsWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::SingleCellSimulationViewContentsWidget *mGui;

    SingleCellSimulationViewGraphPanelsWidget *mGraphPanels;
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
