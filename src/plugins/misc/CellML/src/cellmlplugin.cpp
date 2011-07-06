#include "cellmlplugin.h"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList());
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

} }
