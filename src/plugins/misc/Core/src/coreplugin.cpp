#include "coreplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CorePluginInfo()
{
    PluginInfo info;

    info.type         = Both;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

}
