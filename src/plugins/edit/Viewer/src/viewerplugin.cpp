#include "viewerplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC ViewerPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core" << "QtMmlWidget";

    return res;
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

}
