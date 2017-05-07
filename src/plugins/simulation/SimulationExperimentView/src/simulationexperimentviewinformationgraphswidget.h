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
// Simulation Experiment view information graphs widget
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

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewPlugin;
class SimulationExperimentViewSimulation;
class SimulationExperimentViewSimulationWidget;

//==============================================================================

class SimulationExperimentViewInformationGraphsWidget : public QStackedWidget,
                                                        public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewInformationGraphsWidget(SimulationExperimentViewPlugin *pPlugin,
                                                             SimulationExperimentViewSimulationWidget *pSimulationWidget,
                                                             QWidget *pParent);

    virtual void retranslateUi();

    void initialize(SimulationExperimentViewSimulation *pSimulation);
    void finalize();

    void updateGui();

    void finishEditing();

    Core::Properties graphProperties(GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                                     const QString &pFileName = QString()) const;

    int headerCount() const;

    int columnWidth(const int &pIndex) const;
    void setColumnWidth(const int &pIndex, const int &pColumnWidth);

private:
    SimulationExperimentViewPlugin *mPlugin;
    SimulationExperimentViewSimulationWidget *mSimulationWidget;

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

signals:
    void headerSectionResized(const int &pIndex, const int &pOldSize,
                              const int &pNewSize);

    void graphsUpdated(OpenCOR::GraphPanelWidget::GraphPanelPlotWidget *pPlot,
                       const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

public slots:
    void initialize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                    const bool &pActive = true);
    void finalize(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel);

    void addGraph(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                  OpenCOR::GraphPanelWidget::GraphPanelPlotGraph *pGraph);
    void removeGraphs(OpenCOR::GraphPanelWidget::GraphPanelWidget *pGraphPanel,
                      const OpenCOR::GraphPanelWidget::GraphPanelPlotGraphs &pGraphs);

private slots:
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

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
