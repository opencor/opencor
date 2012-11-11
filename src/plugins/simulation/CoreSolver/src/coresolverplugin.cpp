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

    descriptions.insert("en", "The core solver plugin");
    descriptions.insert("fr", "L'extension de solveur de base");

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
