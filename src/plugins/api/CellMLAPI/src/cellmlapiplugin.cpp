//==============================================================================
// CellML API plugin
//==============================================================================

#include "cellmlapiplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAPI {

//==============================================================================

PLUGININFO_FUNC CellMLAPIPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to access the <a href=\"http://cellml-api.sourceforge.net/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour accéder à l'<a href=\"http://cellml-api.sourceforge.net/\">API CellML</a>");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::General,
                          PluginInfo::Api,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLAPI, CellMLAPIPlugin)

//==============================================================================

}   // namespace CellMLAPI
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
