//==============================================================================
// Single cell simulation graph panels widget
//==============================================================================

#include "singlecellsimulationgraphpanel.h"
#include "singlecellsimulationgraphpanels.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationGraphPanels::SingleCellSimulationGraphPanels(QWidget *pParent) :
    QSplitter(Qt::Vertical, pParent)
{
    // Add a default graph panel

    addGraphPanel();
}

//==============================================================================

SingleCellSimulationGraphPanel * SingleCellSimulationGraphPanels::addGraphPanel()
{
    // Create a new graph panel

    SingleCellSimulationGraphPanel *res = new SingleCellSimulationGraphPanel(this);

    // Add it as a widget

    addWidget(res);

    // Create a connection to keep track of whenever the graph panel gets
    // activated

    connect(res, SIGNAL(activated(SingleCellSimulationGraphPanel *)),
            this, SLOT(graphPanelActivated(SingleCellSimulationGraphPanel *)));

    // Activate it

    res->setActive(true);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

void SingleCellSimulationGraphPanels::removeGraphPanel()
{
    // Remove the current graph panel

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel->isActive()) {
            // We are dealing with the currently active graph panel, so remove
            // it

            graphPanel->hide();

            delete graphPanel;

            // Activate the next graph panel or the last one available, if any

            iMax = count();

            if (!iMax)
                // No more graph panel, so...

                return;
            else if (i < iMax)
                // There is a next graph panel, so activate it

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(i))->setActive(true);
            else
                // We were dealing with the last graph panel, but there is still
                // at least one left, so activate the new last graph panel

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(count()-1))->setActive(true);

            // We are all done, so...

            return;
        }
    }
}

//==============================================================================

void SingleCellSimulationGraphPanels::graphPanelActivated(SingleCellSimulationGraphPanel *pGraphPanel)
{
    // A graph panel has been activated, so inactivate all the others

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel != pGraphPanel)
            // We are not dealing with the graph panel that just got activated,
            // so inactivate it

            graphPanel->setActive(false);
    }
}

//==============================================================================

}   // namespace SingleCellSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
