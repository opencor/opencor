/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Single cell view graph panels widget
//==============================================================================

#include "singlecellviewgraphpanelplotwidget.h"
#include "singlecellviewgraphpanelswidget.h"
#include "singlecellviewgraphpanelwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QSettings>
#include <QMetaType>
#include <QWheelEvent>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

SingleCellViewGraphPanelsWidget::SingleCellViewGraphPanelsWidget(QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(pParent),
    mSplitterSizes(QIntList()),
    mActiveGraphPanels(QMap<QString, SingleCellViewGraphPanelWidget *>()),
    mActiveGraphPanel(0),
    mPlotsRects(QMap<QString, QMap<SingleCellViewGraphPanelPlotWidget *, QRectF>>())
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

    for (int i = 0, iMax = count(); i < iMax; ++i)
        qobject_cast<SingleCellViewGraphPanelWidget *>(widget(i))->retranslateUi();
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
    // of graphs
    // Note: we don't assign the value of SettingsGraphPanelSizes directly to
    //       mSplitterSizes because adding a graph panel will reset it. So,
    //       instead, we assign the value to splitterSizes, which we then use to
    //       properly initialise mSplitterSizes...

    QIntList splitterSizes = qVariantListToIntList(pSettings->value(SettingsGraphPanelSizes).toList());
    int graphPanelsCount = splitterSizes.count();

    if (!graphPanelsCount)
        // For some reasons, the settings for the number of graph panels to be
        // created got messed up, so reset it

        graphPanelsCount = 1;

    for (int i = 0; i < graphPanelsCount; ++i)
        addGraphPanel();

    // Retrieve and set the size of each graph panel

    mSplitterSizes = splitterSizes;

    setSizes(mSplitterSizes);
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the size of each graph panel

    pSettings->setValue(SettingsGraphPanelSizes, qIntListToVariantList(mSplitterSizes));
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::initialize(const QString &pFileName)
{
    // Set the active graph panel or select the first one, if no backup exists

    SingleCellViewGraphPanelWidget *activeGraphPanel = mActiveGraphPanels.value(pFileName);

    if (activeGraphPanel)
        activeGraphPanel->setActive(true);
    else
        qobject_cast<SingleCellViewGraphPanelWidget *>(widget(0))->setActive(true);

    // Update our plots' axes' values
    // Note: we always want our plot to be replotted. Indeed, say that you plot
    //       a graph that doesn't require changing the axes' values of the plot
    //       (i.e. they still have their default values), and then you switch to
    //       a different file. In that case, the axes' values won't be updated
    //       and the plot not replotted. That is, unless we replot the plot no
    //       matter what...

    QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> plotsRects = mPlotsRects.value(pFileName);

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellViewGraphPanelPlotWidget *plot = qobject_cast<SingleCellViewGraphPanelWidget *>(widget(i))->plot();

        QRectF dataRect = plotsRects.value(plot);

        if (dataRect == QRectF()) {
            if (!plot->resetAxes())
                plot->replotNow();
        } else {
            if (!plot->setAxes(dataRect))
                plot->replotNow();
        }
    }
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::backup(const QString &pFileName)
{
    // Keep track of our active graph panel

    mActiveGraphPanels.insert(pFileName, mActiveGraphPanel);

    // Keep track of the axes' values of our different plots

    QMap<SingleCellViewGraphPanelPlotWidget *, QRectF> plotsRects = QMap<SingleCellViewGraphPanelPlotWidget *, QRectF>();

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellViewGraphPanelPlotWidget *plot = qobject_cast<SingleCellViewGraphPanelWidget *>(widget(i))->plot();

        plotsRects.insert(plot, QRectF(QPointF(plot->minX(), plot->minY()),
                                       QPointF(plot->maxX(), plot->maxY())));
    }

    mPlotsRects.insert(pFileName, plotsRects);
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::finalize(const QString &pFileName)
{
    // Remove track of our active graph panel and the axes' values of our
    // different plots

    mActiveGraphPanels.remove(pFileName);
    mPlotsRects.remove(pFileName);
}

//==============================================================================

QList<SingleCellViewGraphPanelWidget *> SingleCellViewGraphPanelsWidget::graphPanels() const
{
    // Return all our graph panels

    QList<SingleCellViewGraphPanelWidget *> res = QList<SingleCellViewGraphPanelWidget *>();

    for (int i = 0, iMax = count(); i < iMax; ++i)
        res << qobject_cast<SingleCellViewGraphPanelWidget *>(widget(i));

    return res;
}

//==============================================================================

SingleCellViewGraphPanelWidget * SingleCellViewGraphPanelsWidget::activeGraphPanel() const
{
    // Return our active graph panel

    return mActiveGraphPanel;
}

//==============================================================================

SingleCellViewGraphPanelWidget * SingleCellViewGraphPanelsWidget::addGraphPanel()
{
    // Keep track of the graph panels' original size

    QIntList origSizes = sizes();

    // Create a new graph panel

    SingleCellViewGraphPanelWidget *res = new SingleCellViewGraphPanelWidget(this);

    // Add the graph panel to ourselves

    addWidget(res);

    // Resize the graph panels, thus making sure that their size is what it
    // should be (see issue #58)

    double scalingFactor = double(count()-1)/count();

    for (int i = 0, iMax = origSizes.count(); i < iMax; ++i)
        origSizes[i] *= scalingFactor;

    setSizes(origSizes << height()/count());

    // Keep track of whenever a graph panel gets activated

    connect(res, SIGNAL(activated(SingleCellViewGraphPanelWidget *)),
            this, SIGNAL(graphPanelActivated(SingleCellViewGraphPanelWidget *)));

    connect(res, SIGNAL(activated(SingleCellViewGraphPanelWidget *)),
            this, SLOT(updateGraphPanels(SingleCellViewGraphPanelWidget *)));

    // Keep track of the addition and removal of a graph

    connect(res, SIGNAL(graphAdded(SingleCellViewGraphPanelPlotWidget *, SingleCellViewGraphPanelPlotGraph *)),
            this, SIGNAL(graphAdded(SingleCellViewGraphPanelPlotWidget *, SingleCellViewGraphPanelPlotGraph *)));
    connect(res, SIGNAL(graphsRemoved(SingleCellViewGraphPanelPlotWidget *, const QList<SingleCellViewGraphPanelPlotGraph *> &)),
            this, SIGNAL(graphsRemoved(SingleCellViewGraphPanelPlotWidget *, const QList<SingleCellViewGraphPanelPlotGraph *> &)));

    // Activate the graph panel

    res->setActive(true);

    // Keep track of our new sizes

    splitterMoved();

    // Let people know that we have added a graph panel

    emit graphPanelAdded(res);

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(count() > 1);

    // Return our newly created graph panel

    return res;
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    if (!pGraphPanel)
        return;

    // Retrieve the index of the given graph panel

    int index = indexOf(pGraphPanel);

    // Hide the graph panel and let people know that we have removed it (or,
    // rather, about to remove it)
    // Note: we let people know before we actually delete the graph panel,
    //       because some people interested in that signal might have used the
    //       pointer to keep track of some information, as is done in
    //       SingleCellViewInformationGraphsWidget for example...

    pGraphPanel->hide();

    emit graphPanelRemoved(pGraphPanel);

    // Remove all tracks
    // Note: mActiveGraphPanel will automatically get updated when another graph
    //       panel gets selected...

    foreach (const QString &fileName, mActiveGraphPanels.keys())
        if (mActiveGraphPanels.value(fileName) == pGraphPanel)
            mActiveGraphPanels.remove(fileName);

    // Now, we can delete our graph panel

    delete pGraphPanel;

    // Let people know whether graph panels can be removed

    emit removeGraphPanelsEnabled(count() > 1);

    // Activate the next graph panel or the last one available, if any

    if (index < count())
        // There is a next graph panel, so activate it

        qobject_cast<SingleCellViewGraphPanelWidget *>(widget(index))->setActive(true);
    else
        // We were dealing with the last graph panel, so just activate the new
        // last graph panel

        qobject_cast<SingleCellViewGraphPanelWidget *>(widget(count()-1))->setActive(true);

    // Keep track of our new sizes

    splitterMoved();
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeCurrentGraphPanel()
{
    // Make sure that we don't have only one graph panel left

    if (count() == 1)
        return;

    // Remove the current graph panel

    removeGraphPanel(mActiveGraphPanel);
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::removeAllGraphPanels()
{
    // Make sure that we don't have only one graph panel left

    if (count() == 1)
        return;

    // Remove all the graph panels but one
    // Note: the one we keep is the very first one since it may be the user's
    //       most important graph panel...

    while (count() > 1)
        removeGraphPanel(qobject_cast<SingleCellViewGraphPanelWidget *>(widget(count()-1)));
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::splitterMoved()
{
    // Our splitter has been moved, so keep track of its new sizes

    mSplitterSizes = sizes();
}

//==============================================================================

void SingleCellViewGraphPanelsWidget::updateGraphPanels(SingleCellViewGraphPanelWidget *pGraphPanel)
{
    // Keep track of the newly activated graph panel

    mActiveGraphPanel = pGraphPanel;

    // Inactivate all the other graph panels

    for (int i = 0, iMax = count(); i < iMax; ++i) {
        SingleCellViewGraphPanelWidget *graphPanel = qobject_cast<SingleCellViewGraphPanelWidget *>(widget(i));

        if (graphPanel != pGraphPanel)
            // We are not dealing with the graph panel that just got activated,
            // so inactivate it

            graphPanel->setActive(false);
    }
}

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
