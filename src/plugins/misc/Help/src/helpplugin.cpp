#include "helpplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC HelpPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "Core";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

}
