#include "filebrowserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    PluginInfo info;

    info.type         = Gui;
    info.dependencies = QStringList() << "Core";

    return info;
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

}
