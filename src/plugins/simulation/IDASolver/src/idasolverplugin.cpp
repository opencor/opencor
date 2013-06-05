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

    descriptions.insert("en", QString::fromUtf8("a plugin which uses <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> to solve DAEs."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui utilise <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> pour résoudre des EADs."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Simulation,
                          true,
                          QStringList() << "CoreSolver" << "SUNDIALS",
                          descriptions);
}

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
    Descriptions MaximumStepPropertyDescriptions;
    Descriptions MaximumNumberOfStepsPropertyDescriptions;
    Descriptions RelativeTolerancePropertyDescriptions;
    Descriptions AbsoluteTolerancePropertyDescriptions;

    MaximumStepPropertyDescriptions.insert("en", QString::fromUtf8("Maximum step"));
    MaximumStepPropertyDescriptions.insert("fr", QString::fromUtf8("Pas maximum"));

    MaximumNumberOfStepsPropertyDescriptions.insert("en", QString::fromUtf8("Maximum number of steps"));
    MaximumNumberOfStepsPropertyDescriptions.insert("fr", QString::fromUtf8("Nombre maximum de pas"));

    RelativeTolerancePropertyDescriptions.insert("en", QString::fromUtf8("Relative tolerance"));
    RelativeTolerancePropertyDescriptions.insert("fr", QString::fromUtf8("Tolérance relative"));

    AbsoluteTolerancePropertyDescriptions.insert("en", QString::fromUtf8("Absolute tolerance"));
    AbsoluteTolerancePropertyDescriptions.insert("fr", QString::fromUtf8("Tolérance absolue"));

    res.append(Solver::Property(Solver::Double, MaximumStepId, MaximumStepPropertyDescriptions, MaximumStepDefaultValue, true));
    res.append(Solver::Property(Solver::Integer, MaximumNumberOfStepsId, MaximumNumberOfStepsPropertyDescriptions, MaximumNumberOfStepsDefaultValue));
    res.append(Solver::Property(Solver::Double, RelativeToleranceId, RelativeTolerancePropertyDescriptions, RelativeToleranceDefaultValue));
    res.append(Solver::Property(Solver::Double, AbsoluteToleranceId, AbsoluteTolerancePropertyDescriptions, AbsoluteToleranceDefaultValue));

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
