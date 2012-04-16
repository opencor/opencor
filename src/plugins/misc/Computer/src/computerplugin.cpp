//==============================================================================
// Computer plugin
//==============================================================================

#include "computerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

PLUGININFO_FUNC ComputerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support code compilation");
    descriptions.insert("fr", "Une extension pour supporter la compilation de code");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "LLVM",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Computer, ComputerPlugin)

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
