#include "versionmanagerplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC VersionManagerPluginInfo()
{
    PluginInfo info;

    info.type         = Both;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

}
