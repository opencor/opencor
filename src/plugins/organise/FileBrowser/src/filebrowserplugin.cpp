#include "filebrowserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "Core";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

}
