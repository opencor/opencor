//==============================================================================
// Clang plugin
//==============================================================================

#include "clangplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Clang {

//==============================================================================

PLUGININFO_FUNC ClangPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"http://clang.llvm.org/\">Clang</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"http://clang.llvm.org/\">Clang</a>");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Clang, ClangPlugin)

//==============================================================================

}   // namespace Clang
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
