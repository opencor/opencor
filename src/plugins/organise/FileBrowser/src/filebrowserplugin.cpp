#include "filebrowserplugin.h"

namespace OpenCOR {
namespace FileBrowser {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       QObject::tr("a plugin which provides you with a means to access your local files"));
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

} }
