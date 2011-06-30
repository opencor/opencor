#include "cellmlplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CellMLPluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::General;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

}
