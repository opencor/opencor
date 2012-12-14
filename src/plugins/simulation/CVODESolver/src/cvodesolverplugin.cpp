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

    descriptions.insert("en", QString::fromUtf8("A plugin which uses <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> to solve ODEs"));
    descriptions.insert("fr", QString::fromUtf8("Une extension qui utilise <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> pour résoudre des EDOs"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CVODESolver, CVODESolverPlugin)

//==============================================================================

QString CVODESolverPlugin::name() const
{
    // Return the name of the solver

    return "CVODE";
}

//==============================================================================

Solver::Type CVODESolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Ode;
}

//==============================================================================

Solver::Properties CVODESolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.append(Solver::Property(MaximumStepProperty, Solver::Double, true));
    res.append(Solver::Property(MaximumNumberOfStepsProperty, Solver::Integer));
    res.append(Solver::Property(RelativeToleranceProperty, Solver::Double));
    res.append(Solver::Property(AbsoluteToleranceProperty, Solver::Double));

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
