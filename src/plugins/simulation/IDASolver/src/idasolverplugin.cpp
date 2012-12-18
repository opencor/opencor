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

    descriptions.insert("en", QString::fromUtf8("A plugin which uses <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> to solve DAEs"));
    descriptions.insert("fr", QString::fromUtf8("Une extension qui utilise <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> pour résoudre des EADs"));

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

Solver::Type IDASolverPlugin::type() const
{
    // Return the type of the solver

    return Solver::Dae;
}

//==============================================================================

QString IDASolverPlugin::name() const
{
    // Return the name of the solver

    return "IDA";
}

//==============================================================================

Solver::Properties IDASolverPlugin::properties() const
{
    // Return the properties supported by the solver

    Solver::Properties res = Solver::Properties();

    res.append(Solver::Property(Solver::Double, MaximumStepProperty, true));
    res.append(Solver::Property(Solver::Integer, MaximumNumberOfStepsProperty));
    res.append(Solver::Property(Solver::Double, RelativeToleranceProperty));
    res.append(Solver::Property(Solver::Double, AbsoluteToleranceProperty));

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
