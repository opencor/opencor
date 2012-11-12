//==============================================================================
// Qwt plugin
//==============================================================================

#include "qwtplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Qwt {

//==============================================================================

PLUGININFO_FUNC QwtPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to access <a href=\"http://qwt.sourceforge.net/\">Qwt</a>");
    descriptions.insert("fr", "Une extension pour accéder à <a href=\"http://qwt.sourceforge.net/\">Qwt</a>");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Qwt, QwtPlugin)

//==============================================================================

}   // namespace Qwt
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
