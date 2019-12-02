/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Graph panels widget
//==============================================================================

#include "coreguiutils.h"
#include "graphpanelswidget.h"
#include "i18ninterface.h"

//==============================================================================

#include <QAction>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace GraphPanelWidget {

//==============================================================================

GraphPanelsWidget::GraphPanelsWidget(QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Create our actions

    mSynchronizeXAxisAction = Core::newAction(true, this);
    mSynchronizeYAxisAction = Core::newAction(true, this);

    connect(mSynchronizeXAxisAction, &QAction::triggered,
            this, &GraphPanelsWidget::synchronizeXAxis);
    connect(mSynchronizeYAxisAction, &QAction::triggered,
            this, &GraphPanelsWidget::synchronizeYAxis);

    // Check whether one of our splitters has move and, therefore, whether our
    // sizes have changed

    connect(this, &Core::SplitterWidget::splitterMoved,
            this, &GraphPanelsWidget::stopUsingInternalSizes);
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

    for (auto graphPanel : mGraphPanels) {
        graphPanel->retranslateUi();
    }
}

//==============================================================================

void GraphPanelsWidget::initialize(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties)
{
    // Create a default graph panel, if none exists

    if (mGraphPanels.isEmpty()) {
        addGraphPanel(pGraphPanelWidgetProperties);
    }
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

GraphPanelWidget * GraphPanelsWidget::addGraphPanel(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                                                    bool pActive)
{
    // Create a new graph panel, add it to ourselves and keep track of it

    auto res = new GraphPanelWidget(mGraphPanels, mSynchronizeXAxisAction, mSynchronizeYAxisAction, this);

    mGraphPanels << res;

    // Resize the graph panels, if needed, making sure that their size is what
    // it should be
    // Note: we use our internal sizes for as long as possible (i.e. as long as
    //       none of our splitters has moved; although, we can reuse our
    //       internal sizes if we have only one graph panel left). Indeed, if we
    //       were to open a CellML file and create two graph panels, then their
    //       height would be based on actual heights since everything would be
    //       visible. On the other hand, if we were to open a SED-ML file with
    //       three graph panels and that we were to rely on their actual height
    //       to resize them, then their new height would be based on 'hidden'
    //       heights since nothing would be visible. Now, the problem is that
    //       those 'hidden' heights tend to be much smaller than actual heights.
    //       The end result is that the two sets of graph panels may end up
    //       being quite different, something that we can prevent by using our
    //       internal heights...

    if (mUseInternalSizes) {
        mInternalSizes << 1;

        setSizes(mInternalSizes);
    } else {
        QIntList oldSizes = sizes();
        QIntList newSizes = QIntList();
        int oldTotalHeight = height()-(mGraphPanels.count()-2)*handleWidth();
        int newTotalHeight = oldTotalHeight-handleWidth();
        double scalingFactor = double(mGraphPanels.count()-1)/mGraphPanels.count()*newTotalHeight/oldTotalHeight;

        for (int i = 0, iMax = oldSizes.count()-1; i < iMax; ++i) {
            newSizes << qRound(scalingFactor*oldSizes[i]);
        }

        setSizes(newSizes << newTotalHeight-std::accumulate(newSizes.begin(), newSizes.end(), 0));
    }

    // Keep track of whenever a graph panel gets activated

    connect(res, &GraphPanelWidget::activated,
            this, &GraphPanelsWidget::graphPanelActivated);

    connect(res, &GraphPanelWidget::activated,
            this, &GraphPanelsWidget::updateGraphPanels);

    // Keep track of the addition and removal of a graph

    connect(res, &GraphPanelWidget::graphAdded,
            this, &GraphPanelsWidget::graphAdded);
    connect(res, &GraphPanelWidget::graphsRemoved,
            this, &GraphPanelsWidget::graphsRemoved);

    // Let people know that we have added a graph panel

    emit graphPanelAdded(res, pGraphPanelWidgetProperties, pActive);

    // In/activate the graph panel

    res->setActive(pActive);

    // Synchronise the axes of our graph panels, if needed, and ensure that they
    // are all aligned with one another by forcing the setting of the axes of
    // our newly active graph panel

    GraphPanelPlotWidget *activeGraphPanelPlot = res->plot();

    activeGraphPanelPlot->setAxes(activeGraphPanelPlot->minX(),
                                  activeGraphPanelPlot->maxX(),
                                  activeGraphPanelPlot->minY(),
                                  activeGraphPanelPlot->maxY(),
                                  true, true, true, false, true, true);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

bool GraphPanelsWidget::removeGraphPanel(GraphPanelWidget *pGraphPanel)
{
    if (pGraphPanel == nullptr) {
        return false;
    }

    // Retrieve the index of the given graph panel

    int index = mGraphPanels.indexOf(pGraphPanel);

    // Let people know that we have removed it (or, rather, about to remove it)
    // Note: we let people know before we actually delete the graph panel,
    //       because some people interested in that signal might have used the
    //       pointer to keep track of some information, as is done in
    //       SimulationExperimentViewInformationGraphPanelAndGraphsWidget for
    //       example...

    emit graphPanelRemoved(pGraphPanel);

    // Update some trackers
    // Note: mActiveGraphPanel will automatically get updated when another graph
    //       panel gets selected...

    mGraphPanels.removeOne(pGraphPanel);

    if (mUseInternalSizes) {
        mInternalSizes.removeLast();
    }

    if (mGraphPanels.count() == 1) {
        mUseInternalSizes = true;
    }

    // Now, we can delete our graph panel

    delete pGraphPanel;

    // Activate the next graph panel or the last one available, if any

    if (index < mGraphPanels.count()) {
        // There is a next graph panel, so activate it

        mGraphPanels[index]->setActive(true);
    } else {
        // We were dealing with the last graph panel, so just activate the new
        // last graph panel

        mGraphPanels[mGraphPanels.count()-1]->setActive(true);
    }

    // Ask our active graph panel's plot to align itself against its neighbours,
    // if any

    if (!mGraphPanels.isEmpty()) {
        mActiveGraphPanel->plot()->updateGui();
    }

    return true;
}

//==============================================================================

bool GraphPanelsWidget::removeCurrentGraphPanel(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties)
{
    // Remove the current graph panel

    if (mGraphPanels.count() == 1) {
        // There is only one graph panel, so add one and then remove our 'first'
        // graph panel

        setUpdatesEnabled(false);
            addGraphPanel(pGraphPanelWidgetProperties);

            bool res = removeGraphPanel(mGraphPanels.first());
        setUpdatesEnabled(true);

        return res;
    }

    return removeGraphPanel(mActiveGraphPanel);
}

//==============================================================================

void GraphPanelsWidget::removeAllGraphPanels(const GraphPanelWidgetProperties &pGraphPanelWidgetProperties)
{
    // Remove all the graph panels, after having created an 'empty' one (since
    // we want at least one graph panel at any given point in time)

    setUpdatesEnabled(false);
        addGraphPanel(pGraphPanelWidgetProperties);

        while (mGraphPanels.count() > 1) {
            removeGraphPanel(mGraphPanels.first());
        }
    setUpdatesEnabled(true);
}

//==============================================================================

void GraphPanelsWidget::setActiveGraphPanel(GraphPanelWidget *pGraphPanel)
{
    // Make sure that we own the given graph panel

    if (!mGraphPanels.contains(pGraphPanel)) {
        return;
    }

    // Make the given graph panel the active one

    pGraphPanel->setActive(true, true);
}

//==============================================================================

QIntList GraphPanelsWidget::sizes() const
{
    // Return either our internal or default sizes, depending on our current
    // state

    return mUseInternalSizes?mInternalSizes:QSplitter::sizes();
}

//==============================================================================

void GraphPanelsWidget::setSizes(const QIntList &pSizes)
{
    // Make sure that we have the correct number of sizes

    if (pSizes.count() != count()) {
        return;
    }

    // Set our sizes as internal and/or default sizes, depending on whether the
    // given sizes consist of ones

    for (auto size : pSizes) {
        if (size != 1) {
            mUseInternalSizes = false;

            break;
        }
    }

    if (mUseInternalSizes) {
        mInternalSizes = pSizes;
    }

    QSplitter::setSizes(pSizes);
}

//==============================================================================

void GraphPanelsWidget::stopUsingInternalSizes()
{
    // Stop using our internal sizes

    mUseInternalSizes = false;
}

//==============================================================================

void GraphPanelsWidget::updateGraphPanels(GraphPanelWidget *pGraphPanel)
{
    // Keep track of the newly activated graph panel

    mActiveGraphPanel = pGraphPanel;

    // Inactivate all the other graph panels

    for (auto graphPanel : mGraphPanels) {
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
                                      true, true, true, false, true, false);
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
                                      true, true, true, false, false, true);
    }
}

//==============================================================================

} // namespace GraphPanelWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
