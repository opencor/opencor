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

    descriptions.insert("en", "A plugin to access <a href=\"http://www.llvm.org/\">LLVM</a> (as well as <a href=\"http://clang.llvm.org/\">Clang</a>)");
    descriptions.insert("fr", "Une extension pour accéder à <a href=\"http://www.llvm.org/\">LLVM</a> (ainsi que <a href=\"http://clang.llvm.org/\">Clang</a>)");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(LLVM, LLVMPlugin)

//==============================================================================

}   // namespace LLVM
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
