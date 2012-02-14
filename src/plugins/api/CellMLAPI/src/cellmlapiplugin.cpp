//==============================================================================
// CellML plugin
//==============================================================================

#include "cellmlplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CellML {

//==============================================================================

PLUGININFO_FUNC CellMLPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use the <a href=\"http://www.cellml.org/tools/api/\">CellML API</a>");
    descriptions.insert("fr", "Une extension pour utiliser l'<a href=\"http://www.cellml.org/tools/api/\">API CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::Api,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

//==============================================================================

}   // namespace CellML
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
