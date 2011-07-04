#include "filebrowserplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

}
