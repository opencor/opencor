#include "fileorganiserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::Gui;
    info.dependencies = QStringList() << "Core";

    return info;
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

}
