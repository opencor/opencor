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

    descriptions.insert("en", QString::fromUtf8("a plugin that implements the <a href=\"http://en.wikipedia.org/wiki/Heun's_method\">Heun's method</a> to solve ODEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Heun's_method\">méthode de Heun</a> pour résoudre des EDOs."));

    return new PluginInfo(PluginInfo::Solver, true, false,
                          QStringList(),
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

    if (!pKisaoId.compare("KISAO:0000301"))
        return solverName();
    else if (!pKisaoId.compare("KISAO:0000483"))
        return StepId;

    return QString();
}

//==============================================================================

QString HeunSolverPlugin::kisaoId(const QString &pId) const
{
    // Return the KiSAO id for the given id

    if (!pId.compare(solverName()))
        return "KISAO:0000301";
    else if (!pId.compare(StepId))
        return "KISAO:0000483";

    return QString();
}

//==============================================================================

Solver::Type HeunSolverPlugin::solverType() const
{
    // Return the type of the solver

    return Solver::Ode;
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

    return Solver::Properties() << Solver::Property(Solver::Property::Double, StepId, stepDescriptions, QStringList(), StepDefaultValue, true);
}

//==============================================================================

QMap<QString, bool> HeunSolverPlugin::solverPropertiesVisibility(const QMap<QString, QString> &pSolverPropertiesValues) const
{
    Q_UNUSED(pSolverPropertiesValues);

    // We don't handle this interface...

    return QMap<QString, bool>();
}

//==============================================================================

}   // namespace HeunSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
