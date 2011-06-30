#include "coreplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CorePluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::General;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

}
