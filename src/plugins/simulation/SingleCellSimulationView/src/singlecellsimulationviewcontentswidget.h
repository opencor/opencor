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
class SingleCellSimulationViewInformationWidget;

//==============================================================================

class SingleCellSimulationViewContentsWidget : public QSplitter,
                                               public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewContentsWidget(QWidget *pParent = 0);
    ~SingleCellSimulationViewContentsWidget();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellSimulationViewInformationWidget * informationWidget();
    SingleCellSimulationViewGraphPanelsWidget * graphPanelsWidget();

private:
    Ui::SingleCellSimulationViewContentsWidget *mGui;

    QList<int> mSplitterSizes;

    SingleCellSimulationViewInformationWidget *mInformationWidget;
    SingleCellSimulationViewGraphPanelsWidget *mGraphPanelsWidget;

private Q_SLOTS:
    void splitterMoved();
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
