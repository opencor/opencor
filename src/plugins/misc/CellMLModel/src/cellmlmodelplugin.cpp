//==============================================================================
// CellMLModel plugin
//==============================================================================

#include "cellmlmodelplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLModel {

//==============================================================================

PLUGININFO_FUNC CellMLModelPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to handle <a href=\"http://www.cellml.org/\">CellML</a> models");
    descriptions.insert("fr", "Une extension pour manipuler des modèles <a href=\"http://www.cellml.org/\">CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Miscellaneous,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLModel, CellMLModelPlugin)

//==============================================================================

}   // namespace CellMLModel
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
