//==============================================================================
// RICORDOSupport plugin
//==============================================================================

#include "ricordosupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace RICORDOSupport {

//==============================================================================

PLUGININFO_FUNC RICORDOSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support <a href=\"http://www.ricordo.eu/\">RICORDO</a>");
    descriptions.insert("fr", "Une extension pour supporter <a href=\"http://www.ricordo.eu/\">RICORDO</a>");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(RICORDOSupport, RICORDOSupportPlugin)

//==============================================================================

}   // namespace RICORDOSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
