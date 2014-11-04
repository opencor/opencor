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
// IDASolver plugin
//==============================================================================

#include "idasolver.h"
#include "idasolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

PLUGININFO_FUNC IDASolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that uses <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> to solve DAEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> pour résoudre des EADs."));

    return new PluginInfo(PluginInfo::Solver, true, false,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================


void IDASolverPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Solver interface
//==============================================================================


void * IDASolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new IdaSolver();
}

//==============================================================================

Solver::Type IDASolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Dae;
}

//==============================================================================

QString IDASolverPlugin::solverName() const
{
    // Return the name of the solver

    return "IDA";
}

//==============================================================================

Solver::Properties IDASolverPlugin::solverProperties() const
{
    // Return the properties supported by the solver

    Descriptions MaximumStepDescriptions;
    Descriptions MaximumNumberOfStepsDescriptions;
    Descriptions RelativeToleranceDescriptions;
    Descriptions AbsoluteToleranceDescriptions;
    Descriptions InterpolateSolutionDescriptions;

    MaximumStepDescriptions.insert("en", QString::fromUtf8("Maximum step"));
    MaximumStepDescriptions.insert("fr", QString::fromUtf8("Pas maximum"));

    MaximumNumberOfStepsDescriptions.insert("en", QString::fromUtf8("Maximum number of steps"));
    MaximumNumberOfStepsDescriptions.insert("fr", QString::fromUtf8("Nombre maximum de pas"));

    RelativeToleranceDescriptions.insert("en", QString::fromUtf8("Relative tolerance"));
    RelativeToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance relative"));

    AbsoluteToleranceDescriptions.insert("en", QString::fromUtf8("Absolute tolerance"));
    AbsoluteToleranceDescriptions.insert("fr", QString::fromUtf8("Tolérance absolue"));

    InterpolateSolutionDescriptions.insert("en", QString::fromUtf8("Interpolate solution"));
    InterpolateSolutionDescriptions.insert("fr", QString::fromUtf8("Interpoler solution"));

    return Solver::Properties() << Solver::Property(Solver::Property::Double, MaximumStepId, MaximumStepDescriptions, MaximumStepDefaultValue, true)
                                << Solver::Property(Solver::Property::Integer, MaximumNumberOfStepsId, MaximumNumberOfStepsDescriptions, MaximumNumberOfStepsDefaultValue)
                                << Solver::Property(Solver::Property::Double, RelativeToleranceId, RelativeToleranceDescriptions, RelativeToleranceDefaultValue)
                                << Solver::Property(Solver::Property::Double, AbsoluteToleranceId, AbsoluteToleranceDescriptions, AbsoluteToleranceDefaultValue)
                                << Solver::Property(Solver::Property::Boolean, InterpolateSolutionId, InterpolateSolutionDescriptions, InterpolateSolutionDefaultValue);
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
