//==============================================================================
// LLVM plugin
//==============================================================================

#include "llvmplugin.h"

//==============================================================================

namespace OpenCOR {
namespace LLVM {

//==============================================================================

PLUGININFO_FUNC LLVMPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"http://www.llvm.org/\">LLVM</a> (as well as <a href=\"http://clang.llvm.org/\">Clang</a>)"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"http://www.llvm.org/\">LLVM</a> (ainsi qu'à <a href=\"http://clang.llvm.org/\">Clang</a>)"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace LLVM
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
