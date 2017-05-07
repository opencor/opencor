/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Graph panels widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "graphpanelswidget.h"
#include "i18ninterface.h"

//==============================================================================

#include <QAction>
#include <QSettings>

//==============================================================================

#include "sedmlapibegin.h"
    #include "sedml/SedDocument.h"
#include "sedmlapiend.h"

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelsWidget::GraphPanelsWidget(QWidget *pParent) :
    Core::SplitterWidget(pParent),
    mGraphPanels(GraphPanelWidgets()),
    mActiveGraphPanel(0)
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Create our actions

    mSynchronizeXAxisAction = Core::newAction(true, this);
    mSynchronizeYAxisAction = Core::newAction(true, this);

    connect(mSynchronizeXAxisAction, SIGNAL(triggered(bool)),
            this, SLOT(synchronizeXAxis()));
    connect(mSynchronizeYAxisAction, SIGNAL(triggered(bool)),
            this, SLOT(synchronizeYAxis()));
}

//==============================================================================

void GraphPanelsWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mSynchronizeXAxisAction, tr("Synchonise X Axis"),
                                     tr("Synchronise the X axis of all graph panels"));
    I18nInterface::retranslateAction(mSynchronizeYAxisAction, tr("Synchonise Y Axis"),
                                     tr("Synchronise the Y axis of all graph panels"));

    // Retranslate all our graph panels

    foreach (GraphPanelWidget *graphPanel, mGraphPanels)
        graphPanel->retranslateUi();
}

//==============================================================================

void GraphPanelsWidget::initialize()
{
    // Create a default graph panel, if none exists

    if (mGraphPanels.isEmpty())
        addGraphPanel();
}

//==============================================================================

GraphPanelWidgets GraphPanelsWidget::graphPanels() const
{
    // Return our graph panels

    return mGraphPanels;
}

//==============================================================================

GraphPanelWidget * GraphPanelsWidget::activeGraphPanel() const
{
    // Return our active graph panel

    return mActiveGraphPanel;
}

//==============================================================================

GraphPanelWidget * GraphPanelsWidget::addGraphPanel(const bool &pActive)
{
    // Keep track of the graph panels' original size

    QIntList origSizes = sizes();

    // Create a new graph panel, add it to ourselves and keep track of it

    GraphPanelWidget *res = new GraphPanelWidget(mGraphPanels,
                                                 mSynchronizeXAxisAction,
                                                 mSynchronizeYAxisAction,
                                                 this);

    mGraphPanels << res;

    // Resize the graph panels, thus making sure that their size is what it
    // should be

    double scalingFactor = double(mGraphPanels.count()-1)/mGraphPanels.count();

    for (int i = 0, iMax = origSizes.count(); i < iMax; ++i)
        origSizes[i] *= scalingFactor;

    setSizes(origSizes << height()/mGraphPanels.count());

    // Keep track of whenever a graph panel gets activated

    connect(res, SIGNAL(activated(OpenCOR::GraphPanelWidget::GraphPanelWidget *)),
            this, SIGNAL(graphPanelActivated(OpenCOR::GraphPanelWidget::GraphPanelWidget *)));

    connect(res, SIGNAL(activated(OpenCOR::GraphPanelWidget::GraphPanelWidget *)),
            this, SLOT(updateGraphPanels(OpenCOR::GraphPanelWidget::GraphPanelWidget *)));

    // Keep track of the addition and removal of a graph

    connect(res, SIGNAL(graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)),
            this, SIGNAL(graphAdded(OpenCOR::GraphPanelWidget::GraphPanelWidget *, OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *)));
    connect(res, SIGNAL(graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)),
            this, SIGNAL(graphsRemoved(OpenCOR::GraphPanelWidget::GraphPanelWidget *, const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &)));

    // In/activate the graph panel

    res->setActive(pActive);

    // Let people know that we have added a graph panel

    emit graphPanelAdded(res, pActive);

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(mGraphPanels.count() > 1);

    // Ask our first graph panel's plot to align itself against its neighbours
    // Synchronise the axes of our graph panels, if needed, and ensure that they
    // are all aligned with one another by forcing the setting of the axes of
    // our active graph panel

    GraphPanelPlotWidget *activeGraphPanelPlot = mActiveGraphPanel->plot();

    activeGraphPanelPlot->setAxes(activeGraphPanelPlot->minX(),
                                  activeGraphPanelPlot->maxX(),
                                  activeGraphPanelPlot->minY(),
                                  activeGraphPanelPlot->maxY(),
                                  true, true, true, true);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

bool GraphPanelsWidget::removeGraphPanel(GraphPanelWidget *pGraphPanel)
{
    if (!pGraphPanel)
        return false;

    // Retrieve the index of the given graph panel

    int index = mGraphPanels.indexOf(pGraphPanel);

    // Let people know that we have removed it (or, rather, about to remove it)
    // Note: we let people know before we actually delete the graph panel,
    //       because some people interested in that signal might have used the
    //       pointer to keep track of some information, as is done in
    //       SimulationExperimentViewInformationGraphsWidget for example...

    emit graphPanelRemoved(pGraphPanel);

    // Remove all tracks
    // Note: mActiveGraphPanel will automatically get updated when another graph
    //       panel gets selected...

    mGraphPanels.removeOne(pGraphPanel);

    // Now, we can delete our graph panel

    delete pGraphPanel;

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(mGraphPanels.count() > 1);

    // Activate the next graph panel or the last one available, if any

    if (index < mGraphPanels.count()) {
        // There is a next graph panel, so activate it

        mGraphPanels[index]->setActive(true);
    } else {
        // We were dealing with the last graph panel, so just activate the new
        // last graph panel

        mGraphPanels[mGraphPanels.count()-1]->setActive(true);
    }

    // Ask our first graph panel's plot, if any, to align itself against its
    // neighbours

    if (!mGraphPanels.isEmpty()) {
        mActiveGraphPanel->plot()->forceAlignWithNeighbors();

        return true;
    } else {
        return false;
    }
}

//==============================================================================

bool GraphPanelsWidget::removeCurrentGraphPanel()
{
    // Make sure that we don't have only one graph panel left

    if (mGraphPanels.count() == 1)
        return false;

    // Remove the current graph panel

    return removeGraphPanel(mActiveGraphPanel);
}

//==============================================================================

void GraphPanelsWidget::removeAllGraphPanels()
{
    // Make sure that we don't have only one graph panel left

    if (mGraphPanels.count() == 1)
        return;

    // Remove all the graph panels but one
    // Note: the one we keep is the very first one since it may be the user's
    //       most important graph panel...

    while (mGraphPanels.count() > 1)
        removeGraphPanel(mGraphPanels.last());
}

//==============================================================================

void GraphPanelsWidget::setActiveGraphPanel(GraphPanelWidget *pGraphPanel)
{
    // Make sure that we own the given graph panel

    if (!mGraphPanels.contains(pGraphPanel))
        return;

    // Make the given graph panel the active one

    pGraphPanel->setActive(true);
}

//==============================================================================

void GraphPanelsWidget::updateGraphPanels(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel)
{
    // Keep track of the newly activated graph panel

    mActiveGraphPanel = pGraphPanel;

    // Inactivate all the other graph panels

    foreach (GraphPanelWidget *graphPanel, mGraphPanels) {
        if (graphPanel != pGraphPanel) {
            // We are not dealing with the graph panel that just got activated,
            // so inactivate it

            graphPanel->setActive(false);
        }
    }
}

//==============================================================================

void GraphPanelsWidget::synchronizeXAxis()
{
    // Synchronise the X axis of our graph panels, if needed, by forcing the
    // setting of the axes of our active graph panel

    if (mSynchronizeXAxisAction->isChecked()) {
        GraphPanelPlotWidget *activeGraphPanelPlot = mActiveGraphPanel->plot();

        activeGraphPanelPlot->setAxes(activeGraphPanelPlot->minX(),
                                      activeGraphPanelPlot->maxX(),
                                      activeGraphPanelPlot->minY(),
                                      activeGraphPanelPlot->maxY(),
                                      true, true, true, true);
    }
}

//==============================================================================

void GraphPanelsWidget::synchronizeYAxis()
{
    // Synchronise the Y axis of our graph panels, if needed, by forcing the
    // setting of the axes of our active graph panel

    if (mSynchronizeYAxisAction->isChecked()) {
        GraphPanelPlotWidget *activeGraphPanelPlot = mActiveGraphPanel->plot();

        activeGraphPanelPlot->setAxes(activeGraphPanelPlot->minX(),
                                      activeGraphPanelPlot->maxX(),
                                      activeGraphPanelPlot->minY(),
                                      activeGraphPanelPlot->maxY(),
                                      true, true, true, true);
    }
}

//==============================================================================

}   // namespace GraphPanelWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
