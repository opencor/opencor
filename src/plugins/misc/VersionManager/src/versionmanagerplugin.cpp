#include "versionmanagerplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC VersionManagerPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList();
    info.type         = Both;

    return info;
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

}
