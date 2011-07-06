#include "cellmlplugin.h"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::General;
    res.dependencies = QStringList();

    return res;
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

} }
