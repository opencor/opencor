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
// KINSOL solver plugin
//==============================================================================

#include "kinsolsolver.h"
#include "kinsolsolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

PLUGININFO_FUNC KINSOLSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that uses <a href=\"http://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> to solve non-linear systems."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"http://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> pour résoudre des systèmes non-linéaires."));

    return new PluginInfo(PluginInfo::Solver, true, false,
                          QStringList() << "SUNDIALS",
                          descriptions);
}

//==============================================================================
// Solver interface
//==============================================================================

Solver::Solver * KINSOLSolverPlugin::solverInstance() const
{
    // Create and return an instance of the solver

    return new KinsolSolver();
}

//==============================================================================

QString KINSOLSolverPlugin::id(const QString &pKisaoId) const
{
    // Return the id for the given KiSAO id

    if (!pKisaoId.compare("KISAO:0000282"))
        return solverName();

    return QString();
}

//==============================================================================

QString KINSOLSolverPlugin::kisaoId(const QString &pId) const
{
    // Return the KiSAO id for the given id

    if (!pId.compare(solverName()))
        return "KISAO:0000282";

    return QString();
}

//==============================================================================

Solver::Type KINSOLSolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Nla;
}

//==============================================================================

QString KINSOLSolverPlugin::solverName() const
{
    // Return the name of the solver

    return "KINSOL";
}

//==============================================================================

Solver::Properties KINSOLSolverPlugin::solverProperties() const
{
    // Return the properties supported by the solver, i.e. none in our case

    return Solver::Properties();
}

//==============================================================================

QMap<QString, bool> KINSOLSolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    Q_UNUSED(pSolverPropertiesValues);

    // We don't handle this interface...

    return QMap<QString, bool>();
}

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
