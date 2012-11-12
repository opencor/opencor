//==============================================================================
// ForwardEulerSolver plugin
//==============================================================================

#include "forwardeulersolver.h"
#include "forwardeulersolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

PLUGININFO_FUNC ForwardEulerSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin which implements the <a href=\"http://en.wikipedia.org/wiki/Euler_method\">Forward Euler method</a> to solve ODEs");
    descriptions.insert("fr", "Une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Euler_method\">méthode Forward Euler</a> pour résoudre des EDOs");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(ForwardEulerSolver, ForwardEulerSolverPlugin)

//==============================================================================

QString ForwardEulerSolverPlugin::name() const
{
    // Return the name of the solver

    return "Forward Euler";
}

//==============================================================================

Solver::Type ForwardEulerSolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

Solver::Properties ForwardEulerSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.insert(StepProperty, Solver::Double);

    return res;
}

//==============================================================================

void * ForwardEulerSolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new ForwardEulerSolver();
}

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
