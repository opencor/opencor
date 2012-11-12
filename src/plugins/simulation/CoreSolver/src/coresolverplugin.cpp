//==============================================================================
// CoreSolver plugin
//==============================================================================

#include "corenlasolver.h"
#include "coresolverplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

PLUGININFO_FUNC CoreSolverPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("The core solver plugin"));
    descriptions.insert("fr", QString::fromUtf8("L'extension de solveur de base"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreSolver, CoreSolverPlugin)

//==============================================================================

CoreSolverPlugin::~CoreSolverPlugin()
{
    // Reset our global non-linear algebraic solver

    resetGlobalNlaSolver();
}

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
