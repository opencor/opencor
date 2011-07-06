#include "versionmanagerplugin.h"

namespace OpenCOR {
namespace VersionManager {

PLUGININFO_FUNC VersionManagerPluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList());
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

} }
