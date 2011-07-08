#include "helpplugin.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       QObject::tr("a plugin which provides you help"));
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

} }
