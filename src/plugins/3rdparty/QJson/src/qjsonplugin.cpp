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

    descriptions.insert("en", "A plugin to get access to <a href=\"http://qjson.sourceforge.net/\">QJson</a>");
    descriptions.insert("fr", "Une extension pour avoir accès à <a href=\"http://qjson.sourceforge.net/\">QJson</a>");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QJson, QJsonPlugin)

//==============================================================================

}   // namespace QJson
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
