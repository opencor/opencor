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
// Python wrapper plugin
//==============================================================================

#pragma once

//==============================================================================

#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

#include "PythonQt.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationExperimentView {
    class SimulationExperimentViewWidget;
};

//==============================================================================

namespace PythonWrapper {

//==============================================================================

class PythonWrapperCore;
class PythonWrapperDataStore;
class PythonWrapperSimulationExperimentView;
class PythonWrapperSolver;

//==============================================================================

PLUGININFO_FUNC PythonWrapperPluginInfo();

//==============================================================================

class PythonWrapperPlugin : public QObject, public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.PythonWrapperPlugin" FILE "pythonwrapperplugin.json")

    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit PythonWrapperPlugin();

#include "plugininterface.inl"

    static PythonWrapperPlugin *instance();

    SimulationExperimentView::SimulationExperimentViewWidget *simulationExperimentViewWidget();

private:
    PythonQtObjectPtr mOpenCORModule;

    PythonWrapperCore *mPythonWrapperCore;
    PythonWrapperDataStore *mPythonWrapperDataStore;
    PythonWrapperSimulationExperimentView *mPythonWrapperSimulationExperimentView;
    PythonWrapperSolver *mPythonWrapperSolver;

    SimulationExperimentView::SimulationExperimentViewWidget *mSimulationExperimentViewWidget;
};

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
