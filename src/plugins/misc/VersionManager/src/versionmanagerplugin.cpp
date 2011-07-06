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

void VersionManagerPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void VersionManagerPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

} }
