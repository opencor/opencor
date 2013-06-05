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

    descriptions.insert("en", QString::fromUtf8("a plugin to support code compilation."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour supporter la compilation de code."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "LLVM",
                          descriptions);
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
