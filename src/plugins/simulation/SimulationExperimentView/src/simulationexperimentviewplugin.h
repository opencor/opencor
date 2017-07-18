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
// Simulation Experiment view plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"
#include "pythoninterface.h"
#include "simulationexperimentviewglobal.h"
#include "viewinterface.h"

//==============================================================================

namespace OpenCOR {
namespace SimulationExperimentView {

//==============================================================================

PLUGININFO_FUNC SimulationExperimentViewPluginInfo();

//==============================================================================

class SimulationExperimentViewPythonWrapper;
class SimulationExperimentViewWidget;

//==============================================================================

class SIMULATIONEXPERIMENTVIEW_EXPORT SimulationExperimentViewPlugin : public QObject,
                                                                       public FileHandlingInterface,
                                                                       public I18nInterface,
                                                                       public PluginInterface,
                                                                       public PythonInterface,
                                                                       public ViewInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SimulationExperimentViewPlugin" FILE "simulationexperimentviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::PythonInterface)
    Q_INTERFACES(OpenCOR::ViewInterface)

public:
#include "filehandlinginterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"
#include "pythoninterface.inl"
#include "viewinterface.inl"

    static SimulationExperimentViewPlugin * instance();

    SimulationExperimentViewWidget * viewWidget() const;

private:
    SimulationExperimentViewWidget *mViewWidget;

    SimulationExperimentViewPythonWrapper *mSimulationExperimentViewPythonWrapper;
};

//==============================================================================

}   // namespace SimulationExperimentView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
