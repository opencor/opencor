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

    descriptions.insert("en", "An implementation of the Forward Euler method to solve ODEs");
    descriptions.insert("fr", "Une implémentation de la méthode Forward Euler pour résoudres des EDOs");

    return PluginInfo(PluginInfo::V001,
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

    return tr("Forward Euler");
}

//==============================================================================

ForwardEulerSolverPlugin::SolverInterfaceType ForwardEulerSolverPlugin::type() const
{
    // Return the type of the solver

    return ForwardEulerSolverPlugin::Ode;
}

//==============================================================================

OpenCOR::SolverInterface::Properties ForwardEulerSolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Properties res = Properties();

    res.insert(StepProperty, OpenCOR::SolverInterface::Double);

    return res;
}

//==============================================================================

void * ForwardEulerSolverPlugin::newSolver() const
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
