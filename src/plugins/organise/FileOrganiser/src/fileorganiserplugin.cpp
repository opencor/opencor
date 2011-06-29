#include "fileorganiserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "Core";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

}
