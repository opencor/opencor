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

    int graphPanelsCount() const;

    SingleCellSimulationGraphPanel * activeGraphPanel();

private:
    int mGraphPanelsCount;

Q_SIGNALS:
    void grapPanelAdded(SingleCellSimulationGraphPanel *);
    void grapPanelRemoved();

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
