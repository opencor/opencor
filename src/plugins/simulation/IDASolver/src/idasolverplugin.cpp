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

    descriptions.insert("en", "A plugin which uses <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> to solve DAEs");
    descriptions.insert("fr", "Une extension qui utilise <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> pour résoudres des EDAs");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(IDASolver, IDASolverPlugin)

//==============================================================================

QString IDASolverPlugin::name() const
{
    // Return the name of the solver

    return "IDA";
}

//==============================================================================

Solver::Type IDASolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Dae;
}

//==============================================================================

Solver::Properties IDASolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.insert(MaximumStepProperty, Solver::Double);
    res.insert(MaximumNumberOfStepsProperty, Solver::Integer);
    res.insert(RelativeToleranceProperty, Solver::Double);
    res.insert(AbsoluteToleranceProperty, Solver::Double);

    return res;
}

//==============================================================================

void * IDASolverPlugin::instance() const
{
    // Create and return an instance of the solver

    return new IdaSolver();
}

//==============================================================================

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
