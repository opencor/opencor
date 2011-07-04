#include "helpplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC HelpPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

}
