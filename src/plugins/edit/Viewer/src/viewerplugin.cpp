#include "viewerplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC ViewerPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "Core" << "QtMmlWidget";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

}
