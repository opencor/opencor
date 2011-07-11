#include "fileorganiserplugin.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to organise your files");
    descriptions.insert("fr", "Une extension pour organiser vos fichiers");

    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

} }
