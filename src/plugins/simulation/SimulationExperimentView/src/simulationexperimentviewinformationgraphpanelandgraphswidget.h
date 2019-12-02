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
// Simulation Experiment view information graph panel and graphs widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfileruntime.h"
#include "commonwidget.h"
#include "corecliutils.h"
#include "graphpanelplotwidget.h"
#include "graphpanelwidget.h"
#include "propertyeditorwidget.h"

//==============================================================================

#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
} // namespace CellMLSupport

//==============================================================================

namespace DataStore {
    class DataStoreImportData;
} // namespace DataStore

//==============================================================================

namespace SimulationSupport {
    class Simulation;
} // namespace SimulationSupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewSimulationWidget;
class SimulationExperimentViewWidget;

//==============================================================================

class SimulationExperimentViewInformationGraphPanelAndGraphsWidget : public QStackedWidget,
                                                                     public Core::CommonWidget
{
    Q_OBJECT

public:
    enum class Mode {
        GraphPanel,
        Graphs
    };

    explicit SimulationExperimentViewInformationGraphPanelAndGraphsWidget(SimulationExperimentViewWidget *pViewWidget,
                                                                          SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                                          QWidget *pParent);

    void retranslateUi() override;

    void initialize(SimulationSupport::Simulation *pSimulation);
    void finalize();

    void importData(DataStore::DataStoreImportData *pImportData);

    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);

    void updateGui();

    void finishEditing();

    Core::PropertyEditorWidget * graphPanelPropertyEditor(GraphPanelWidget::GraphPanelWidget *pGraphPanel) const;
    Core::PropertyEditorWidget * graphsPropertyEditor(GraphPanelWidget::GraphPanelWidget *pGraphPanel) const;

    Core::Properties graphsProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                      const QString &pFileName) const;

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode mode() const;
    void setMode(Mode pMode);

    void setGraphPanelColumnWidth(int pIndex, int pColumnWidth);
    void setGraphsColumnWidth(int pIndex, int pColumnWidth);

    void setGraphPanelSectionExpanded(int pSection, bool pExpanded);

    void reinitialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel);

private:
    Mode mMode = Mode::Graphs;

    SimulationExperimentViewWidget *mViewWidget;
    SimulationExperimentViewSimulationWidget *mSimulationWidget;

    SimulationSupport::Simulation *mSimulation = nullptr;

    QMap<Core::PropertyEditorWidget *, GraphPanelWidget::GraphPanelWidget *> mGraphPanels;
    QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *> mGraphPanelPropertyEditors;
    QMap<GraphPanelWidget::GraphPanelWidget *, Core::PropertyEditorWidget *> mGraphsPropertyEditors;
    Core::PropertyEditorWidget *mGraphPanelPropertyEditor = nullptr;
    Core::PropertyEditorWidget *mGraphsPropertyEditor = nullptr;

    QMap<Core::Property *, GraphPanelWidget::GraphPanelPlotGraph *> mGraphs;
    QMap<GraphPanelWidget::GraphPanelPlotGraph *, Core::Property *> mGraphProperties;

    QMenu *mGraphPanelContextMenu;

    QAction *mSelectGraphPanelColorAction;

    QMenu *mGraphContextMenu;
    QMenu *mGraphParametersContextMenu;

    QAction *mAddGraphAction;
    QAction *mRemoveCurrentGraphAction;
    QAction *mRemoveAllGraphsAction;
    QAction *mSelectAllGraphsAction;
    QAction *mUnselectAllGraphsAction;
    QAction *mSelectGraphColorAction;

    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    QMap<QString, QString> mRenamedModelListValues;

    QMenu *mImportMenu = nullptr;

    void retranslateGraphPanelPropertyEditor(Core::PropertyEditorWidget *pGraphPanelPropertyEditor);

    void populateGraphPanelPropertyEditor();

    void populateGraphParametersContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    bool checkParameter(CellMLSupport::CellmlFileRuntime *pRuntime,
                        GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                        Core::Property *pParameterProperty,
                        bool pParameterX) const;

    QString modelListValue(const QString &pFileName) const;

    void updateGraphInfo(Core::Property *pProperty);
    void updateGraphsInfo(Core::Property *pSectionProperty = nullptr);
    void updateAllGraphsInfo();

    void selectAllGraphs(bool pSelect);

    bool rootProperty(Core::Property *pProperty) const;

    void initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    const GraphPanelWidget::GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                    bool pActive, bool pCustomize);

signals:
    void graphPanelGraphsModeChanged(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode pMode);

    void graphPanelHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);
    void graphsHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);

    void graphPanelSectionExpanded(int pSection, bool pExpanded);

    void graphUpdated(GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void graphsUpdated(const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

public slots:
    void initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    const GraphPanelWidget::GraphPanelWidgetProperties &pGraphPanelWidgetProperties,
                    bool pActive);
    void initialize(GraphPanelWidget::GraphPanelWidget *pGraphPanel);
    void finalize(GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                  GraphPanelWidget::GraphPanelPlotGraph *pGraph,
                  const GraphPanelWidget::GraphPanelPlotGraphProperties &pGraphProperties);
    void addGraph();

    void removeGraphs(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                      const GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

    void toggleGraph(GraphPanelWidget::GraphPanelPlotGraph *pGraph);

    void toggleLegend();

    void toggleLogarithmicXAxis();
    void toggleLogarithmicYAxis();

private slots:
    void selectGraphPanelColor();

    void removeCurrentGraph();
    void removeAllGraphs();
    void selectAllGraphs();
    void unselectAllGraphs();
    void selectGraphColor();

    void showGraphPanelContextMenu(const QPoint &pPosition) const;
    void showGraphsContextMenu(const QPoint &pPosition) const;

    void graphPanelSectionExpanded(const QModelIndex &pIndex);
    void graphPanelSectionCollapsed(const QModelIndex &pIndex);

    void graphPanelPropertyChanged(Core::Property *pProperty);
    void graphsPropertyChanged(Core::Property *pProperty);

    void updateParameterValue();
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
