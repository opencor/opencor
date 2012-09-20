//==============================================================================
// CoreSolver plugin
//==============================================================================

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

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::Simulation,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreSolver, CoreSolverPlugin)

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
