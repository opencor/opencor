/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view graph panels widget
//==============================================================================

#include "singlecellviewgraphpanelswidget.h"
#include "singlecellviewsimulationwidget.h"

//==============================================================================

#include <QSettings>

//==============================================================================

#include "sedmlapidisablewarnings.h"
    #include "sedml/SedDocument.h"
#include "sedmlapienablewarnings.h"

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewGraphPanelsWidget::SingleCellViewGraphPanelsWidget(SingleCellViewSimulationWidget *pSimulationWidget,
                                                                 QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(),
    mSimulationWidget(pSimulationWidget),
    mSplitterSizes(QIntList()),
    mGraphPanels(SingleCellViewGraphPanelWidgets()),
    mActiveGraphPanel(0)
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement
    // Note: we need to keep track of our movement so that saveSettings() can
    //       work fine even when we are not visible (which happens when a CellML
    //       file cannot be run for some reason or another)...

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(splitterMoved()));
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::retranslateUi()
{
    // Retranslate all our graph panels

    foreach (SingleCellViewGraphPanelWidget *graphPanel, mGraphPanels)
        graphPanel->retranslateUi();
}

//==============================================================================

static const auto SettingsGraphPanelSizes = QStringLiteral("GraphPanelSizes");

//==============================================================================

void SingleCellViewGraphPanelsWidget::loadSettings(QSettings *pSettings)
{
    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emit removeGraphPanelsEnabled(false);

    // Retrieve the number of graph panels and create the corresponding number
    // of graphs, if we are dealing with a CellML file, or only one default
    // graph panel
    // Note: we don't assign the value of SettingsGraphPanelSizes directly to
    //       mSplitterSizes because adding a graph panel will reset it. So,
    //       instead, we assign the value to splitterSizes, which we then use to
    //       properly initialise mSplitterSizes...

    if (mSimulationWidget->fileType() == SingleCellViewWidget::CellmlFile) {
        QIntList splitterSizes = QIntList();

        pSettings->beginGroup(mSimulationWidget->fileName());
            splitterSizes = qVariantListToIntList(pSettings->value(SettingsGraphPanelSizes).toList());
        pSettings->endGroup();

        int graphPanelsCount = splitterSizes.count();

        if (!graphPanelsCount) {
            // For some reasons, the settings for the number of graph panels to be
            // created got messed up, so reset it

            graphPanelsCount = 1;
        }

        for (int i = 0; i < graphPanelsCount; ++i)
            addGraphPanel();

        // Retrieve and set the size of each graph panel

        mSplitterSizes = splitterSizes;

        setSizes(mSplitterSizes);
    } else {
        addGraphPanel();
    }
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the size of each graph panel, but only if we are dealing
    // with a CellML file

    if (mSimulationWidget->fileType() == SingleCellViewWidget::CellmlFile) {
        pSettings->beginGroup(mSimulationWidget->fileName());
            pSettings->setValue(SettingsGraphPanelSizes, qIntListToVariantList(mSplitterSizes));
        pSettings->endGroup();
    }
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::initialize()
{
    // Set the first graph panel

    qobject_cast<SingleCellViewGraphPanelWidget *>(widget(0))->setActive(true);
}

//==============================================================================

SingleCellViewGraphPanelWidgets SingleCellViewGraphPanelsWidget::graphPanels() const
{
    // Return our graph panels

    return mGraphPanels;
}

//==============================================================================

SingleCellViewGraphPanelWidget * SingleCellViewGraphPanelsWidget::activeGraphPanel() const
{
    // Return our active graph panel

    return mActiveGraphPanel;
}

//==============================================================================

SingleCellViewGraphPanelWidget * SingleCellViewGraphPanelsWidget::addGraphPanel(const bool &pActive)
{
    // Keep track of the graph panels' original size

    QIntList origSizes = sizes();

    // Create a new graph panel, add it to ourselves and keep track of it

    SingleCellViewGraphPanelWidget *res = new SingleCellViewGraphPanelWidget(mGraphPanels, this);

    mGraphPanels << res;

    // Resize the graph panels, thus making sure that their size is what it
    // should be

    double scalingFactor = double(mGraphPanels.count()-1)/mGraphPanels.count();

    for (int i = 0, iMax = origSizes.count(); i < iMax; ++i)
        origSizes[i] *= scalingFactor;

    setSizes(origSizes << height()/mGraphPanels.count());

    // Keep track of whenever a graph panel gets activated

    connect(res, SIGNAL(activated(SingleCellViewGraphPanelWidget *)),
            this, SIGNAL(graphPanelActivated(SingleCellViewGraphPanelWidget *)));

    connect(res, SIGNAL(activated(SingleCellViewGraphPanelWidget *)),
            this, SLOT(updateGraphPanels(SingleCellViewGraphPanelWidget *)));

    // Keep track of the addition and removal of a graph

    connect(res, SIGNAL(graphAdded(SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotGraph *)),
            this, SIGNAL(graphAdded(SingleCellViewGraphPanelWidget *, SingleCellViewGraphPanelPlotGraph *)));
    connect(res, SIGNAL(graphsRemoved(SingleCellViewGraphPanelWidget *, const SingleCellViewGraphPanelPlotGraphs &)),
            this, SIGNAL(graphsRemoved(SingleCellViewGraphPanelWidget *, const SingleCellViewGraphPanelPlotGraphs &)));

    // In/activate the graph panel

    res->setActive(pActive);

    // Keep track of our new sizes

    splitterMoved();

    // Let people know that we have added a graph panel

    emit graphPanelAdded(res, pActive);

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(mGraphPanels.count() > 1);

    // Ask our first graph panel's plot to align itself against its neighbours

    mGraphPanels[0]->plot()->forceAlignWithNeighbors();

    // Return our newly created graph panel

    return res;
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    if (!pGraphPanel)
        return;

    // Retrieve the index of the given graph panel

    int index = mGraphPanels.indexOf(pGraphPanel);

    // Let people know that we have removed it (or, rather, about to remove it)
    // Note: we let people know before we actually delete the graph panel,
    //       because some people interested in that signal might have used the
    //       pointer to keep track of some information, as is done in
    //       SingleCellViewInformationGraphsWidget for example...

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

    if (!mGraphPanels.isEmpty())
        mGraphPanels[0]->plot()->forceAlignWithNeighbors();

    // Keep track of our new sizes

    splitterMoved();
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeCurrentGraphPanel()
{
    // Make sure that we don't have only one graph panel left

    if (mGraphPanels.count() == 1)
        return;

    // Remove the current graph panel

    removeGraphPanel(mActiveGraphPanel);
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeAllGraphPanels()
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

void SingleCellViewGraphPanelsWidget::setActiveGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Make sure that we own the given graph panel

    if (!mGraphPanels.contains(pGraphPanel))
        return;

    // Make the given graph panel the active one

    pGraphPanel->setActive(true);
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::splitterMoved()
{
    // Our splitter has been moved, so keep track of its new sizes

    if (mSimulationWidget->fileType() == SingleCellViewWidget::CellmlFile)
        mSplitterSizes = sizes();
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::updateGraphPanels(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Keep track of the newly activated graph panel

    mActiveGraphPanel = pGraphPanel;

    // Inactivate all the other graph panels

    foreach (SingleCellViewGraphPanelWidget *graphPanel, mGraphPanels) {
        if (graphPanel != pGraphPanel) {
            // We are not dealing with the graph panel that just got activated,
            // so inactivate it

            graphPanel->setActive(false);
        }
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
