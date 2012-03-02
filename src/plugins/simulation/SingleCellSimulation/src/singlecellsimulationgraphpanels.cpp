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
    CommonWidget(pName, this, pParent),
    mGraphPanelsCount(0)
{
}

//==============================================================================

static const QString SettingsGraphPanelsCount = "GraphPanelsCount";

//==============================================================================

void SingleCellSimulationGraphPanels::loadSettings(QSettings *pSettings)
{
    // Retrieve the number of graph panels and create the corresponding number
    // of graphs

    int graphPanelsCount = pSettings->value(SettingsGraphPanelsCount).toInt();

    if (!graphPanelsCount)
        // We left the previous session with no graph panel, so add one as a
        // default

        graphPanelsCount = 1;

    for (int i = 0; i < graphPanelsCount; ++i)
        addGraphPanel();

    // Select the first graph panel

    qobject_cast<SingleCellSimulationGraphPanel *>(widget(0))->setActive(true);
}

//==============================================================================

void SingleCellSimulationGraphPanels::saveSettings(QSettings *pSettings) const
{
    // Keep track of the number of graph panels

    pSettings->setValue(SettingsGraphPanelsCount, mGraphPanelsCount);
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
    // Check whether there is already a graph panel and if not, then delete our
    // dummy widget which ensures that we don't shrink

    if (!mGraphPanelsCount && count()) {
        // No previous graph panel, so...
        // Note: the test for count() above is for the very first time we want
        //       to add a graph panel and therefore where no dummy widget will
        //       be present...

        QWidget *dummyWidget = qobject_cast<QWidget*>(widget(0));

        dummyWidget->hide();

        delete dummyWidget;
    }

    // Create a new graph panel

    SingleCellSimulationGraphPanel *res = new SingleCellSimulationGraphPanel(this);

    // Add the graph panel to ourselves

    addWidget(res);

    // Keep track of the fact that we are holding one more graph panel

    ++mGraphPanelsCount;

    // Create a connection to keep track of whenever the graph panel gets
    // activated

    connect(res, SIGNAL(activated(SingleCellSimulationGraphPanel *)),
            this, SLOT(graphPanelActivated(SingleCellSimulationGraphPanel *)));

    // Activate it

    res->setActive(true);

    // Let people know that we have added a graph panel

    emit grapPanelAdded(res);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

void SingleCellSimulationGraphPanels::removeGraphPanel()
{
    if (!mGraphPanelsCount)
        // We don't have any graph panel to remove, so...

        return;

    // Remove the current graph panel

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellSimulationGraphPanel *graphPanel = qobject_cast<SingleCellSimulationGraphPanel *>(widget(i));

        if (graphPanel->isActive()) {
            // We are dealing with the currently active graph panel, so remove
            // it

            graphPanel->hide();

            delete graphPanel;

            // Keep track of the fact that we are holding one less graph panel

            --mGraphPanelsCount;

            // Activate the next graph panel or the last one available, if any

            iMax = count();

            if (!iMax)
                // No more graph panel, so...

                break;
            else if (i < iMax)
                // There is a next graph panel, so activate it

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(i))->setActive(true);
            else
                // We were dealing with the last graph panel, but there is still
                // at least one left, so activate the new last graph panel

                qobject_cast<SingleCellSimulationGraphPanel *>(widget(count()-1))->setActive(true);

            // We are all done, so...

            break;
        }
    }

    // Check whether we have any graph panel left and if so add a dummy widget
    // which will ensure that we don't shrink

    if (!mGraphPanelsCount)
        addWidget(new QWidget(this));

    // Let people know that we have removed a graph panel

    emit grapPanelRemoved();
}

//==============================================================================

int SingleCellSimulationGraphPanels::graphPanelsCount() const
{
    // Return the number of graph panels we currently hold

    return mGraphPanelsCount;
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
