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
// Single Cell view information graphs widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileruntime.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "graphpanelplotwidget.h"
#include "propertyeditorwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
}   // namespace CellMLSupport

//==============================================================================

namespace GraphPanelWidget {
    class GraphPanelWidget;
}   // namespace GraphPanelWidget

//==============================================================================

namespace SingleCellView {

//==============================================================================

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

    virtual void retranslateUi();

    void initialize(SingleCellViewSimulation *pSimulation);
    void finalize();

    void updateGui();

    void finishEditing();

    Core::Properties graphProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                     const QString &pFileName = QString()) const;

    int headerCount() const;

    int columnWidth(const int &pIndex) const;
    void setColumnWidth(const int &pIndex, const int &pColumnWidth);

private:
    SingleCellViewPlugin *mPlugin;
    SingleCellViewSimulationWidget *mSimulationWidget;

    QMap<Core::PropertyEditorWidget *, GraphPanelWidget::GraphPanelWidget *> mGraphPanels;
    QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *> mPropertyEditors;
    Core::PropertyEditorWidget *mPropertyEditor;

    QMap<Core::Property *, GraphPanelWidget::GraphPanelPlotGraph *> mGraphs;
    QMap<GraphPanelWidget::GraphPanelPlotGraph *, Core::Property *> mGraphProperties;

    QMenu *mContextMenu;
    QMenu *mParametersContextMenu;

    QAction *mAddGraphAction;
    QAction *mRemoveCurrentGraphAction;
    QAction *mRemoveAllGraphsAction;
    QAction *mSelectAllGraphsAction;
    QAction *mUnselectAllGraphsAction;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    bool mCanEmitGraphsUpdatedSignal;

    int mHorizontalScrollBarValue;

    void populateParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                        GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                        Core::Property *pParameterProperty,
                        const bool &pParameterX) const;

    void updateGraphInfo(Core::Property *pProperty, const QString &pFileName);
    void updateGraphsInfo(Core::Property *pSectionProperty = 0);
    void updateAllGraphsInfo();

    void selectAllGraphs(const bool &pSelect);

Q_SIGNALS:
    void headerSectionResized(const int &pIndex, const int &pOldSize,
                              const int &pNewSize);

    void graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

public Q_SLOTS:
    void initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    const bool &pActive = true);
    void finalize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                  OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void removeGraphs(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                      const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

private Q_SLOTS:
    void addGraph();
    void removeCurrentGraph();
    void removeAllGraphs();
    void selectAllGraphs();
    void unselectAllGraphs();

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
