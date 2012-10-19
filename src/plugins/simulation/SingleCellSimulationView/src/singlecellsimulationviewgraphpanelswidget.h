//==============================================================================
// Single cell simulation view graph panels widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONVIEWGRAPHPANELSWIDGET_H
#define SINGLECELLSIMULATIONVIEWGRAPHPANELSWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace Ui {
    class SingleCellSimulationViewGraphPanelsWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulationView {

//==============================================================================

class SingleCellSimulationViewGraphPanelWidget;

//==============================================================================

class SingleCellSimulationViewGraphPanelsWidget : public QSplitter,
                                                  public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationViewGraphPanelsWidget(QWidget *pParent);
    ~SingleCellSimulationViewGraphPanelsWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellSimulationViewGraphPanelWidget * addGraphPanel();
    void removeGraphPanel();

    SingleCellSimulationViewGraphPanelWidget * activeGraphPanel();

    void clearGraphPanels();
    void clearActiveGraphPanel();

protected:
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    Ui::SingleCellSimulationViewGraphPanelsWidget *mGui;

Q_SIGNALS:
    void grapPanelAdded(SingleCellSimulationViewGraphPanelWidget *pGraphPanel);
    void grapPanelRemoved();

    void removeGraphPanelsEnabled(const bool &pEnabled);

private Q_SLOTS:
    void graphPanelActivated(SingleCellSimulationViewGraphPanelWidget *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellSimulationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
