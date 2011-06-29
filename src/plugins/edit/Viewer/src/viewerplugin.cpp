#include "viewerplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC ViewerPluginInfo()
{
    PluginInfo info;

    info.type         = Gui;
    info.dependencies = QStringList() << "Core" << "QtMmlWidget";

    return info;
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

}
