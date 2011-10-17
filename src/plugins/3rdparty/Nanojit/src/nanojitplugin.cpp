//==============================================================================
// Nanojit plugin
//==============================================================================

#include "nanojitplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Nanojit {

//==============================================================================

PLUGININFO_FUNC NanojitPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"https://developer.mozilla.org/En/Nanojit/\">Nanojit</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"https://developer.mozilla.org/En/Nanojit/\">Nanojit</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::ThirdParty,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Nanojit, NanojitPlugin)

//==============================================================================

}   // namespace Nanojit
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
