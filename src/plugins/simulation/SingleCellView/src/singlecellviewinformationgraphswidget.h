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
// Single cell view information graphs widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "corecliutils.h"
#include "propertyeditorwidget.h"
#include "singlecellviewgraphpanelplotwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace Ui {
    class SingleCellViewInformationGraphsWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace SingleCellView {

//==============================================================================

class SingleCellViewGraphPanelWidget;
class SingleCellViewPlugin;
class SingleCellViewSimulation;
class SingleCellViewSimulationWidget;

//==============================================================================

class SingleCellViewInformationGraphsWidget : public QStackedWidget,
                                              public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SingleCellViewInformationGraphsWidget(SingleCellViewPlugin *pPlugin,
                                                   SingleCellViewSimulationWidget *pSimulationWidget,
                                                   QWidget *pParent);
    ~SingleCellViewInformationGraphsWidget();

    virtual void retranslateUi();

    void initialize(SingleCellViewSimulation *pSimulation);
    void finalize();

    void updateGui();

    void finishEditing();

    Core::Properties graphProperties(SingleCellViewGraphPanelWidget *pGraphPanel,
                                     const QString &pFileName = QString()) const;

    int headerCount() const;

    int columnWidth(const int &pIndex) const;
    void setColumnWidth(const int &pIndex, const int &pColumnWidth);

private:
    Ui::SingleCellViewInformationGraphsWidget *mGui;

    SingleCellViewPlugin *mPlugin;
    SingleCellViewSimulationWidget *mSimulationWidget;

    QMap<Core::PropertyEditorWidget *, SingleCellViewGraphPanelWidget *> mGraphPanels;
    QMap<SingleCellViewGraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;
    Core::PropertyEditorWidget *mPropertyEditor;

    QMap<Core::Property *, SingleCellViewGraphPanelPlotGraph *> mGraphs;
    QMap<SingleCellViewGraphPanelPlotGraph *, Core::Property *> mGraphProperties;

    QMenu *mContextMenu;
    QMenu *mParametersContextMenu;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    bool mCanEmitGraphsUpdatedSignal;

    int mHorizontalScrollBarValue;

    void populateParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                        SingleCellViewGraphPanelPlotGraph *pGraph,
                        Core::Property *pParameterProperty,
                        const bool &pParameterX) const;

    void updateGraphInfo(Core::Property *pProperty, const QString &pFileName);
    void updateGraphsInfo(Core::Property *pSectionProperty = 0);
    void updateAllGraphsInfo();

    void selectAllGraphs(const bool &pSelect);

Q_SIGNALS:
    void headerSectionResized(const int &pIndex, const int &pOldSize,
                              const int &pNewSize);

    void graphsUpdated(SingleCellViewGraphPanelPlotWidget *pPlot,
                       const SingleCellViewGraphPanelPlotGraphs &pGraphs);

public Q_SLOTS:
    void initialize(SingleCellViewGraphPanelWidget *pGraphPanel,
                    const bool &pActive = true);
    void finalize(SingleCellViewGraphPanelWidget *pGraphPanel);

    void addGraph(SingleCellViewGraphPanelWidget *pGraphPanel,
                  SingleCellViewGraphPanelPlotGraph *pGraph);
    void removeGraphs(SingleCellViewGraphPanelWidget *pGraphPanel,
                      const SingleCellViewGraphPanelPlotGraphs &pGraphs);

private Q_SLOTS:
    void on_actionAddGraph_triggered();

    void on_actionRemoveCurrentGraph_triggered();
    void on_actionRemoveAllGraphs_triggered();

    void on_actionSelectAllGraphs_triggered();
    void on_actionUnselectAllGraphs_triggered();

    void propertyEditorContextMenu(const QPoint &pPosition) const;

    void propertyEditorHorizontalScrollBarValueChanged(const int &pValue);

    void propertyEditorSectionResized(const int &pIndex, const int &pOldSize,
                                      const int &pNewSize);

    void graphChanged(Core::Property *pProperty);

    void updateParameterValue();
};

//==============================================================================

}   // namespace SingleCellView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
