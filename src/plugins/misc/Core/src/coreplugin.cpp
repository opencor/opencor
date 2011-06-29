#include "coreplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CorePluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList();
    info.type         = Both;

    return info;
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

}
