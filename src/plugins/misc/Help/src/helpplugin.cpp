#include "helpplugin.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to provide help");
    descriptions.insert("fr", "Une extension pour fournir de l'aide");

    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

} }
