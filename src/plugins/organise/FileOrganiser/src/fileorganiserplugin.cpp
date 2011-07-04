#include "fileorganiserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

}
