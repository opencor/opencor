//==============================================================================
// Compiler plugin
//==============================================================================

#include "compilerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

PLUGININFO_FUNC CompilerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support code compilation");
    descriptions.insert("fr", "Une extension pour supporter la compilation de code");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "CoreSolver" << "LLVM",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Compiler, CompilerPlugin)

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
