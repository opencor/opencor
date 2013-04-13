//==============================================================================
// MidpointSolver plugin
//==============================================================================

#include "midpointsolver.h"
#include "midpointsolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace MidpointSolver {

//==============================================================================

PLUGININFO_FUNC MidpointSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin which implements the <a href=\"http://en.wikipedia.org/wiki/Midpoint_method\">midpoint method</a> to solve ODEs"));
    descriptions.insert("fr", QString::fromUtf8("Une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Midpoint_method\">méthode du point médian</a> pour résoudre des EDOs"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver",
                          descriptions);
}

//==============================================================================

Solver::Type MidpointSolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString MidpointSolverPlugin::name() const
{
    // Return the name of the solver

    return "Midpoint";
}

//==============================================================================

Solver::Properties MidpointSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.append(Solver::Property(Solver::Double, StepProperty, DefaultStep, true));

    return res;
}

//==============================================================================

void * MidpointSolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new MidpointSolver();
}

//==============================================================================

}   // namespace MidpointSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
