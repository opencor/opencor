//==============================================================================
// Single cell simulation graph panels widget
//==============================================================================

#include "singlecellsimulationgraphpanel.h"
#include "singlecellsimulationgraphpanels.h"

//==============================================================================

#include <QSettings>
#include <QWheelEvent>

//==============================================================================

namespace OpenCOR {
namespace SingleCellSimulation {

//==============================================================================

SingleCellSimulationGraphPanels::SingleCellSimulationGraphPanels(const QString &pName,
                                                                 QWidget *pParent) :
    QSplitter(Qt::Vertical, pParent),
    CommonWidget(pName, this, pParent)
{
}

//==============================================================================

static const QString SettingsGraphPanelsCount = "GraphPanelsCount";

//==============================================================================

void SingleCellSimulationGraphPanels::loadSettings(QSettings *pSettings)
{
    // Retrieve the number of graph panels and create the corresponding number
    // of graphs

    int graphPanelsCount = pSettings->value(SettingsGraphPanelsCount, 1).toInt();

    if (!graphPanelsCount)
        // For some reason, the settings for the number of graph panels to be
        // created got messed up, so...

        graphPanelsCount = 1;

    for (int i = 0; i < graphPanelsCount; ++i)
        addGraphPanel();

    // Select the first graph panel

    qobject_cast<SingleCellSimulationGraphPanel *>(widget(0))->setActive(true);

    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit removeGraphPanelsEnabled(false);
}

//==============================================================================

void SingleCellSimulationGraphPanels::saveSettings(QSettings *pSettings) const
{
    // Keep track of the number of graph panels

    pSettings->setValue(SettingsGraphPanelsCount, count());
}

//==============================================================================

void SingleCellSimulationGraphPanels::wheelEvent(QWheelEvent *pEvent)
{
    // Default handling of the event

    QSplitter::wheelEvent(pEvent);

    // Select the previous/next graph panel, if any

    if (pEvent->delta())
    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel->isActive()) {
            // We are dealing with the currently active graph panel, so
            // inactivate it and activate either its predecessor or successor

            graphPanel->setActive(false);

            int shift = 1;

#ifdef Q_WS_MAC
    #ifdef AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER
            // From version 10.7 of Mac OS X, the scrolling works the other way
            // round, so...

            shift = -1;
    #endif
#endif

            i += (pEvent->delta() < 0)?shift:-shift;

            if (i < 0)
                i = 0;
            else if (i == iMax)
                i = iMax-1;

            qobject_cast<SingleCellSimulationGraphPanel *>(widget(i))->setActive(true);

            break;
        }
    }
}

//==============================================================================

SingleCellSimulationGraphPanel * SingleCellSimulationGraphPanels::addGraphPanel()
{
    // Keep track of the graph panels' original size

    QList<int> origSizes = sizes();

    // Create a new graph panel

    SingleCellSimulationGraphPanel *res = new SingleCellSimulationGraphPanel(this);

    // Add the graph panel to ourselves

    addWidget(res);

    // Resize the graph panels, thus making sure that their size is what it
    // should be (see issue #58)

    double scalingFactor = (double) (count()-1)/count();

    for (int i = 0, iMax = origSizes.count(); i < iMax; ++i)
        origSizes[i] *= scalingFactor;

    setSizes(origSizes << height()/count());

    // Create a connection to keep track of whenever the graph panel gets
    // activated

    connect(res, SIGNAL(activated(SingleCellSimulationGraphPanel *)),
            this, SLOT(graphPanelActivated(SingleCellSimulationGraphPanel *)));

    // Activate the graph panel

    res->setActive(true);

    // Let people know that we have added a graph panel

    emit grapPanelAdded(res);

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(count() > 1);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

void SingleCellSimulationGraphPanels::removeGraphPanel()
{
    if (count() == 1)
        // There is only one graph panel left, so...

        return;

    // Remove the current graph panel

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel->isActive()) {
            // We are dealing with the currently active graph panel, so remove
            // it

            graphPanel->hide();

            delete graphPanel;

            // Activate the next graph panel or the last one available, if any

            if (i < count())
                // There is a next graph panel, so activate it

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(i))->setActive(true);
            else
                // We were dealing with the last graph panel, so just activate
                // the new last graph panel

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(count()-1))->setActive(true);

            // We are all done, so...

            break;
        }
    }

    // Let people know that we have removed a graph panel

    emit grapPanelRemoved();

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(count() > 1);
}

//==============================================================================

SingleCellSimulationGraphPanel * SingleCellSimulationGraphPanels::activeGraphPanel()
{
    // Return the active graph panel

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel->isActive())
            // We found the active graph panel, so...

            return graphPanel;
    }

    // There are no graph panels, so...
    // Note: indeed, since if there is at least one graph panel, then we have an
    //       active graph panel...

    return 0;
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
