//==============================================================================
// QJson plugin
//==============================================================================

#include "qjsonplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QJson {

//==============================================================================

PLUGININFO_FUNC QJsonPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"http://qjson.sourceforge.net/\">QJson</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"http://qjson.sourceforge.net/\">QJson</a>"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace QJson
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
