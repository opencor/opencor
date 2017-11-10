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
#include "viewwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationSupport {
    class Simulation;
}   // namespace SimulationSupport

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

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void initialize(const QString &pFileName);
    void finalize(const QString &pFileName);

    QIcon fileTabIcon(const QString &pFileName) const;

    bool saveFile(const QString &pOldFileName, const QString &pNewFileName);

    void fileOpened(const QString &pFileName);
    void filePermissionsChanged(const QString &pFileName);
    void fileModified(const QString &pFileName);
    void fileReloaded(const QString &pFileName);
    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileClosed(const QString &pFileName);

    QStringList fileNames() const;

    Plugins cellmlEditingViewPlugins() const;
    Plugins cellmlSimulationViewPlugins() const;

    SimulationExperimentViewSimulationWidget * simulationWidget(const QString &pFileName) const;
    SimulationSupport::Simulation * simulation(const QString &pFileName) const;
    CellMLSupport::CellmlFileRuntime * runtime(const QString &pFileName) const;

    virtual QWidget * widget(const QString &pFileName);

    qulonglong simulationResultsSize(const QString &pFileName) const;

    void checkSimulationResults(const QString &pFileName,
                                const bool &pClearGraphs = false);

private:
    SimulationExperimentViewPlugin *mPlugin;

    Plugins mCellmlEditingViewPlugins;
    Plugins mCellmlSimulationViewPlugins;

    QIntList mSimulationWidgetSizes;
    QIntList mContentsWidgetSizes;

    QBoolList mCollapsibleWidgetCollapsed;

    SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode mGraphPanelGraphsMode;

    QIntList mSimulationColumnWidths;
    QIntList mSolversColumnWidths;
    QIntList mGraphPanelColumnWidths;
    QIntList mGraphsColumnWidths;
    QIntList mParametersColumnWidths;

    QMap<int, bool> mGraphPanelSectionsExpanded;

    SimulationExperimentViewSimulationWidget *mSimulationWidget;
    QMap<QString, SimulationExperimentViewSimulationWidget *> mSimulationWidgets;

    QStringList mFileNames;

    QMap<QString, qulonglong> mSimulationResultsSizes;
    QStringList mSimulationCheckResults;

    void updateContentsInformationGui(SimulationExperimentViewSimulationWidget *pSimulationWidget);

private slots:
    void simulationWidgetSplitterMoved(const QIntList &pSizes);
    void contentsWidgetSplitterMoved(const QIntList &pSizes);

    void collapsibleWidgetCollapsed(const int &pIndex, const bool &pCollapsed);

    void graphPanelSettingsRequested();
    void graphsSettingsRequested();

    void graphPanelGraphsModeChanged(const OpenCOR::SimulationExperimentView::SimulationExperimentViewInformationGraphPanelAndGraphsWidget::Mode &pMode);

    void simulationHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                        const int &pNewSize);
    void solversHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                     const int &pNewSize);
    void graphPanelHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                        const int &pNewSize);
    void graphsHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                    const int &pNewSize);
    void parametersHeaderSectionResized(const int &pIndex, const int &pOldSize,
                                        const int &pNewSize);

    void graphPanelSectionExpanded(const int &pSection, const bool &pExpanded);

    void callCheckSimulationResults();
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
