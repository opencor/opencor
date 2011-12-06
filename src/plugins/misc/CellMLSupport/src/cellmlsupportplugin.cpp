//==============================================================================
// CellMLSupport plugin
//==============================================================================

#include "cellmlsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

PLUGININFO_FUNC CellMLSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to support <a href=\"http://www.cellml.org/\">CellML</a>");
    descriptions.insert("fr", "Une extension pour supporter <a href=\"http://www.cellml.org/\">CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Miscellaneous,
                      false,
                      QStringList() << "CellML" << "LLVM",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLSupport, CellMLSupportPlugin)

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
