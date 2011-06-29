#include "helpplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC HelpPluginInfo()
{
    PluginInfo info;

    info.type         = Gui;
    info.dependencies = QStringList() << "Core";

    return info;
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

}
