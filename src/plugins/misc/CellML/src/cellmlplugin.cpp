#include "cellmlplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CellMLPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList();
    info.type         = Both;

    return info;
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

}
