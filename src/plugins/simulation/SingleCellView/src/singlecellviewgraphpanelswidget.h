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

#ifndef SINGLECELLVIEWGRAPHPANELSWIDGET_H
#define SINGLECELLVIEWGRAPHPANELSWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QMap>
#include <QRectF>
#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelPlotWidget;
class SingleCellViewGraphPanelWidget;

//==============================================================================

class SingleCellViewGraphPanelsWidget : public QSplitter,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewGraphPanelsWidget(QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize(const QString &pFileName);
    void backup(const QString &pFileName);
    void finalize(const QString &pFileName);

    QList<SingleCellViewGraphPanelWidget *> graphPanels() const;
    SingleCellViewGraphPanelWidget * activeGraphPanel() const;

    SingleCellViewGraphPanelWidget * addGraphPanel();

    void removeCurrentGraphPanel();
    void removeAllGraphPanels();

private:
    QIntList mSplitterSizes;

    QMap<QString, SingleCellViewGraphPanelWidget *> mActiveGraphPanels;
    SingleCellViewGraphPanelWidget *mActiveGraphPanel;

    QMap<QString, QMap<SingleCellViewGraphPanelPlotWidget *, QRectF>> mPlotsRects;

    void removeGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel);

Q_SIGNALS:
    void graphPanelAdded(SingleCellViewGraphPanelWidget *pGraphPanel);
    void graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel);

    void removeGraphPanelsEnabled(const bool &pEnabled);

    void graphPanelActivated(SingleCellViewGraphPanelWidget *pGraphPanel);

    void graphAdded(SingleCellViewGraphPanelPlotWidget *pPlot,
                    SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

private Q_SLOTS:
    void splitterMoved();

    void updateGraphPanels(SingleCellViewGraphPanelWidget *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
