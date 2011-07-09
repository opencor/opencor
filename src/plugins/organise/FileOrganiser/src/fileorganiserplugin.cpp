#include "fileorganiserplugin.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       QObject::tr("A plugin to organise your files"));
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

} }
