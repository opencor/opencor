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

    descriptions.insert("en", QString::fromUtf8("A plugin to support code compilation"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour supporter la compilation de code"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "CoreSolver" << "LLVM",
                          descriptions);
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
