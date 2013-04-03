//==============================================================================
// FourthOrderRungeKuttaSolver plugin
//==============================================================================

#include "fourthorderrungekuttasolver.h"
#include "fourthorderrungekuttasolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace FourthOrderRungeKuttaSolver {

//==============================================================================

PLUGININFO_FUNC FourthOrderRungeKuttaSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin which implements the <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">fourth-order Runge-Kutta method</a> to solve ODEs"));
    descriptions.insert("fr", QString::fromUtf8("Une extension qui implémente la <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">méthode Runge-Kutta du quatrième ordre</a> pour résoudre des EDOs"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver",
                          descriptions);
}

//==============================================================================

Solver::Type FourthOrderRungeKuttaSolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

QString FourthOrderRungeKuttaSolverPlugin::name() const
{
    // Return the name of the solver

    return "Runge-Kutta (4th order)";
}

//==============================================================================

Solver::Properties FourthOrderRungeKuttaSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.append(Solver::Property(Solver::Double, StepProperty, DefaultStep, true));

    return res;
}

//==============================================================================

void * FourthOrderRungeKuttaSolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new FourthOrderRungeKuttaSolver();
}

//==============================================================================

}   // namespace FourthOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
