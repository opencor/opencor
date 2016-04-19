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

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "singlecellviewgraphpanelwidget.h"

//==============================================================================

#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewSimulationWidget;

//==============================================================================

class SingleCellViewGraphPanelsWidget : public QSplitter,
                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewGraphPanelsWidget(SingleCellViewSimulationWidget *pSimulationWidget,
                                             QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void initialize();

    SingleCellViewGraphPanelWidgets graphPanels() const;
    SingleCellViewGraphPanelWidget * activeGraphPanel() const;

    SingleCellViewGraphPanelWidget * addGraphPanel(const bool &pActive = true);

    void removeCurrentGraphPanel();
    void removeAllGraphPanels();

    void setActiveGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel);

private:
    SingleCellViewSimulationWidget *mSimulationWidget;

    QIntList mSplitterSizes;

    SingleCellViewGraphPanelWidgets mGraphPanels;

    SingleCellViewGraphPanelWidget *mActiveGraphPanel;

    void removeGraphPanel(SingleCellViewGraphPanelWidget *pGraphPanel);

Q_SIGNALS:
    void graphPanelAdded(SingleCellViewGraphPanelWidget *pGraphPanel,
                         const bool &pActive);
    void graphPanelRemoved(SingleCellViewGraphPanelWidget *pGraphPanel);

    void removeGraphPanelsEnabled(const bool &pEnabled);

    void graphPanelActivated(SingleCellViewGraphPanelWidget *pGraphPanel);

    void graphAdded(SingleCellViewGraphPanelWidget *pGraphPanel,
                    SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(SingleCellViewGraphPanelWidget *pGraphPanel,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

private Q_SLOTS:
    void splitterMoved();

    void updateGraphPanels(SingleCellViewGraphPanelWidget *pGraphPanel);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
