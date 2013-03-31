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

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"http://qwt.sourceforge.net/\">Qwt</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"http://qwt.sourceforge.net/\">Qwt</a>"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace Qwt
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
