#include "versionmanagerplugin.h"

namespace OpenCOR {
namespace VersionManager {

PLUGININFO_FUNC VersionManagerPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::General;
    res.dependencies = QStringList();

    return res;
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

} }
