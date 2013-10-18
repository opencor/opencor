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
// CVODESolver plugin
//==============================================================================

#include "cvodesolver.h"
#include "cvodesolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

PLUGININFO_FUNC CVODESolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin which uses <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> to solve ODEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> pour résoudre des EDOs."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Simulation,
                          false,
                          true,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

//==============================================================================

void CVODESolverPlugin::retranslateUi()
{
    // Nothing to retranslate as such...
}

//==============================================================================

Solver::Type CVODESolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString CVODESolverPlugin::name() const
{
    // Return the name of the solver

    return "CVODE";
}

//==============================================================================

Solver::Properties CVODESolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();
    Descriptions MaximumStepDescriptions;
    Descriptions MaximumNumberOfStepsDescriptions;
    Descriptions RelativeToleranceDescriptions;
    Descriptions AbsoluteToleranceDescriptions;

    MaximumStepDescriptions.insert("en", QString::fromUtf8("Maximum step"));
    MaximumStepDescriptions.insert("fr", QString::fromUtf8("Pas maximum"));

    MaximumNumberOfStepsDescriptions.insert("en", QString::fromUtf8("Maximum number of steps"));
    MaximumNumberOfStepsDescriptions.insert("fr", QString::fromUtf8("Nombre maximum de pas"));

    RelativeToleranceDescriptions.insert("en", QString::fromUtf8("Relative tolerance"));
    RelativeToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance relative"));

    AbsoluteToleranceDescriptions.insert("en", QString::fromUtf8("Absolute tolerance"));
    AbsoluteToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance absolue"));

    res.append(Solver::Property(Solver::Double, MaximumStepId, MaximumStepDescriptions, MaximumStepDefaultValue, true));
    res.append(Solver::Property(Solver::Integer, MaximumNumberOfStepsId, MaximumNumberOfStepsDescriptions, MaximumNumberOfStepsDefaultValue));
    res.append(Solver::Property(Solver::Double, RelativeToleranceId, RelativeToleranceDescriptions, RelativeToleranceDefaultValue));
    res.append(Solver::Property(Solver::Double, AbsoluteToleranceId, AbsoluteToleranceDescriptions, AbsoluteToleranceDefaultValue));

    return res;
}

//==============================================================================

void * CVODESolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new CvodeSolver();
}

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
