#include "fileorganiserplugin.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core");
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

} }
