//==============================================================================
// HeunSolver plugin
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

    descriptions.insert("en", QString::fromUtf8("a plugin which implements the <a href=\"http://en.wikipedia.org/wiki/Heun's_method\">Heun's method</a> to solve ODEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Heun's_method\">méthode de Heun</a> pour résoudre des EDOs."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Simulation,
                          false,
                          true,
                          QStringList() << "CoreSolver",
                          descriptions);
}

//==============================================================================

Solver::Type HeunSolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString HeunSolverPlugin::name() const
{
    // Return the name of the solver

    return "Heun";
}

//==============================================================================

Solver::Properties HeunSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();
    Descriptions stepPropertyDescriptions;

    stepPropertyDescriptions.insert("en", QString::fromUtf8("Step"));
    stepPropertyDescriptions.insert("fr", QString::fromUtf8("Pas"));

    res.append(Solver::Property(Solver::Double, StepId, stepPropertyDescriptions, StepDefaultValue, true));

    return res;
}

//==============================================================================

void * HeunSolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new HeunSolver();
}

//==============================================================================

}   // namespace HeunSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
