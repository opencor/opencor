#include "versionmanagerplugin.h"

namespace OpenCOR {
namespace VersionManager {

PLUGININFO_FUNC VersionManagerPluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList());
}

void VersionManagerPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void VersionManagerPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void VersionManagerPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void VersionManagerPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(VersionManager, VersionManagerPlugin)

} }
