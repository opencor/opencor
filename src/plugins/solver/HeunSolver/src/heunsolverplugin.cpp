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
// Heun solver plugin
//==============================================================================

#include "heunsolver.h"
#include "heunsolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace HeunSolver {

//==============================================================================

PLUGININFO_FUNC HeunSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8(R"(a plugin that implements the <a href="https://en.wikipedia.org/wiki/Heun's_method">Heun's method</a> to solve <a href="https://en.wikipedia.org/wiki/Ordinary_differential_equation">ODEs</a>.)"));
    descriptions.insert("fr", QString::fromUtf8(R"(une extension qui implémente la <a href="https://en.wikipedia.org/wiki/Heun's_method">méthode de Heun</a> pour résoudre des <a href="https://en.wikipedia.org/wiki/Ordinary_differential_equation">EDOs</a>.)"));

    return new PluginInfo(PluginInfo::Category::Solver, true, false,
                          {},
                          descriptions);
}

//==============================================================================
// I18n interface
//==============================================================================

void HeunSolverPlugin::retranslateUi()
{
    // We don't handle this interface...
    // Note: even though we don't handle this interface, we still want to
    //       support it since some other aspects of our plugin are
    //       multilingual...
}

//==============================================================================
// Solver interface
//==============================================================================

Solver::Solver * HeunSolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new HeunSolver();
}

//==============================================================================

QString HeunSolverPlugin::id(const QString &pKisaoId) const
{
    // Return the id for the given KiSAO id

    static const QString Kisao0000301 = "KISAO:0000301";
    static const QString Kisao0000483 = "KISAO:0000483";

    if (pKisaoId == Kisao0000301) {
        return solverName();
    }

    if (pKisaoId == Kisao0000483) {
        return StepId;
    }

    return {};
}

//==============================================================================

QString HeunSolverPlugin::kisaoId(const QString &pId) const
{
    // Return the KiSAO id for the given id

    if (pId == solverName()) {
        return "KISAO:0000301";
    }

    if (pId == StepId) {
        return "KISAO:0000483";
    }

    return {};
}

//==============================================================================

Solver::Type HeunSolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Type::Ode;
}

//==============================================================================

QString HeunSolverPlugin::solverName() const
{
    // Return the name of the solver

    return "Heun";
}

//==============================================================================

Solver::Properties HeunSolverPlugin::solverProperties() const
{
    // Return the properties supported by the solver

    Descriptions stepDescriptions;

    stepDescriptions.insert("en", QString::fromUtf8("Step"));
    stepDescriptions.insert("fr", QString::fromUtf8("Pas"));

    return { Solver::Property(Solver::Property::Type::DoubleGt0, StepId, stepDescriptions, {}, StepDefaultValue, true) };
}

//==============================================================================

QMap<QString, bool> HeunSolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    Q_UNUSED(pSolverPropertiesValues)

    // We don't handle this interface...

    return {};
}

//==============================================================================

} // namespace HeunSolver
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
