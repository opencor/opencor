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

    descriptions.insert("en", "A plugin to use <a href=\"http://qjson.sourceforge.net/\">QJson</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"http://qjson.sourceforge.net/\">QJson</a>");

    return PluginInfo(PluginInfo::V001,
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
