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
// Simulation Experiment view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "combinearchive.h"
#include "corecliutils.h"
#include "sedmlfile.h"
#include "simulationexperimentviewinformationgraphpanelandgraphswidget.h"
#include "simulationexperimentviewsimulationwidget.h"
#include "viewwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationSupport {
    class Simulation;
} // namespace SimulationSupport

//==============================================================================

namespace SimulationExperimentView {

//==============================================================================

class SimulationExperimentViewPlugin;
class SimulationExperimentViewSimulationWidget;

//==============================================================================

class SimulationExperimentViewWidget : public Core::ViewWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewWidget(SimulationExperimentViewPlugin *pPlugin,
                                            const Plugins &pCellmlEditingViewPlugins,
                                            const Plugins &pCellmlSimulationViewPlugins,
                                            QWidget *pParent);

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void retranslateUi() override;

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    bool importFile(const QString &pFileName);
    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    void fileOpened(const QString &pFileName);
    void filePermissionsChanged(const QString &pFileName);
    void fileSaved(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    QStringList fileNames() const;

    Plugins cellmlEditingViewPlugins() const;
    Plugins cellmlSimulationViewPlugins() const;

    SimulationExperimentViewSimulationWidget * simulationWidget() const;
    SimulationExperimentViewSimulationWidget * simulationWidget(const QString &pFileName) const;
    SimulationSupport::Simulation * simulation(const QString &pFileName) const;
    CellMLSupport::CellmlFileRuntime * runtime(const QString &pFileName) const;

    QWidget * widget(const QString &pFileName) override;

    quint64 simulationResultsSize(const QString &pFileName) const;

    void checkSimulationResults(const QString &pFileName,
                                SimulationExperimentViewSimulationWidget::Task pTask = SimulationExperimentViewSimulationWidget::Task::None);

private:
    SimulationExperimentViewPlugin *mPlugin;

    Plugins mCellmlEditingViewPlugins;
    Plugins mCellmlSimulationViewPlugins;

    QIntList mSimulationWidgetSizes;
    QIntList mContentsWidgetSizes;

    QBoolList mCollapsibleWidgetCollapsed;

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode mGraphPanelGraphsMode = SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode::Graphs;

    QIntList mSimulationColumnWidths;
    QIntList mSolversColumnWidths;
    QIntList mGraphPanelColumnWidths;
    QIntList mGraphsColumnWidths;
    QIntList mParametersColumnWidths;

    QMap<int, bool> mGraphPanelSectionsExpanded;

    SimulationExperimentViewSimulationWidget *mSimulationWidget = nullptr;
    QMap<QString, SimulationExperimentViewSimulationWidget *> mSimulationWidgets;

    QStringList mFileNames;

    QMap<QString, quint64> mSimulationResultsSizes;

    void updateContentsInformationGui(SimulationExperimentViewSimulationWidget *pSimulationWidget);

private slots:
    void simulationWidgetSplitterMoved(const QIntList &pSizes);
    void contentsWidgetSplitterMoved(const QIntList &pSizes);

    void collapsibleWidgetCollapsed(int pIndex, bool pCollapsed);

    void graphPanelSettingsRequested();
    void graphsSettingsRequested();

    void graphPanelGraphsModeChanged(SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode pMode);

    void simulationHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);
    void solversHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);
    void graphPanelHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);
    void graphsHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);
    void parametersHeaderSectionResized(int pIndex, int pOldSize, int pNewSize);

    void graphPanelSectionExpanded(int pSection, bool pExpanded);
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
