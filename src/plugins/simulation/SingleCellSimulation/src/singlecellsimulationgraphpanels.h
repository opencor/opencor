//==============================================================================
// Single cell simulation graph panels widget
//==============================================================================

#ifndef SINGLECELLSIMULATIONGRAPHPANELS_H
#define SINGLECELLSIMULATIONGRAPHPANELS_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

class SingleCellSimulationGraphPanel;

//==============================================================================

class SingleCellSimulationGraphPanels : public QSplitter,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellSimulationGraphPanels(const QString &pName,
                                             QWidget *pParent = 0);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    SingleCellSimulationGraphPanel * addGraphPanel();
    void removeGraphPanel();

    SingleCellSimulationGraphPanel * activeGraphPanel();

protected:
    virtual void wheelEvent(QWheelEvent *pEvent);

Q_SIGNALS:
    void grapPanelAdded(SingleCellSimulationGraphPanel *pGraphPanel);
    void grapPanelRemoved();
    void removeGraphPanelsEnabled(const bool &pEnabled);

private Q_SLOTS:
    void graphPanelActivated(SingleCellSimulationGraphPanel *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
