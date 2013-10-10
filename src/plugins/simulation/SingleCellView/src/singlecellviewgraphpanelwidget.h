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
// Single cell view graph panel widget
//==============================================================================

#ifndef SINGLECELLVIEWGRAPHPANELWIDGET_H
#define SINGLECELLVIEWGRAPHPANELWIDGET_H

//==============================================================================

#include "widget.h"

//==============================================================================

#include <QList>

//==============================================================================

class QFrame;

//==============================================================================

namespace Ui {
    class SingleCellViewGraphPanelWidget;
}

//==============================================================================

namespace OpenCOR {
namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelPlotGraph;
class SingleCellViewGraphPanelPlotWidget;

//==============================================================================

class SingleCellViewGraphPanelWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit SingleCellViewGraphPanelWidget(QWidget *pParent = 0);
    ~SingleCellViewGraphPanelWidget();

    virtual void retranslateUi();

    bool isActive() const;
    void setActive(const bool &pActive);

    SingleCellViewGraphPanelPlotWidget * plot() const;

    QList<SingleCellViewGraphPanelPlotGraph *> graphs() const;

    void addGraph(SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraphs(const QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void mousePressEvent(QMouseEvent *pEvent);

private:
    Ui::SingleCellViewGraphPanelWidget *mGui;

    QFrame *mMarker;
    SingleCellViewGraphPanelPlotWidget *mPlot;

    bool mActive;

    void updateMarkerColor();

Q_SIGNALS:
    void activated(SingleCellViewGraphPanelWidget *pGraphPanel);
    void inactivated(SingleCellViewGraphPanelWidget *pGraphPanel);

    void graphAdded(SingleCellViewGraphPanelPlotGraph *pGraph);
    void graphsRemoved(QList<SingleCellViewGraphPanelPlotGraph *> &pGraphs);
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
