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

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"http://qwt.sourceforge.net/\">Qwt</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à <a href=\"http://qwt.sourceforge.net/\">Qwt</a>."));

    return new PluginInfo(PluginInfo::ThirdParty, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace Qwt
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
