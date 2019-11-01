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
// Simulation Experiment view information parameters widget
//==============================================================================

#pragma once

//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace CellMLSupport {
    class CellmlFileRuntime;
    class CellmlFileRuntimeParameter;
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

class SimulationExperimentViewInformationParametersWidget : public Core::PropertyEditorWidget
{
    Q_OBJECT

public:
    explicit SimulationExperimentViewInformationParametersWidget(QWidget *pParent);

    void retranslateUi() override;

    void initialize(SimulationSupport::Simulation *pSimulation,
                    bool pReloadingView = false);
    void finalize();

    void importData(DataStore::DataStoreImportData *pImportData);

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> parameters() const;

protected:
    void contextMenuEvent(QContextMenuEvent *pEvent) override;

private:
    QMenu *mContextMenu;
    QAction *mPlotAgainstVoiMenuAction = nullptr;
    QMenu *mPlotAgainstMenu = nullptr;

    QMap<Core::Property *, CellMLSupport::CellmlFileRuntimeParameter *> mParameters;
    QMap<QAction *, CellMLSupport::CellmlFileRuntimeParameter *> mParameterActions;

    SimulationSupport::Simulation *mSimulation = nullptr;

    bool mNeedClearing = false;
    bool mVoiAccessible = false;

    Core::Property *mImportComponent = nullptr;
    QMenu *mImportMenu = nullptr;

    void populateModel(CellMLSupport::CellmlFileRuntime *pRuntime);
    void populateContextMenu(CellMLSupport::CellmlFileRuntime *pRuntime);

    void updateExtraInfos();

    void retranslateContextMenu();

signals:
    void graphRequired(CellMLSupport::CellmlFileRuntimeParameter *pParameterX,
                       CellMLSupport::CellmlFileRuntimeParameter *pParameterY);

public slots:
    void updateParameters(double pCurrentPoint);

private slots:
    void propertyChanged(Core::Property *pProperty);

    void emitGraphRequired();
};

//==============================================================================

} // namespace SimulationExperimentView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
