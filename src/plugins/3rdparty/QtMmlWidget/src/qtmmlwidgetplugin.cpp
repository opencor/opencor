#include "qtmmlwidgetplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::Gui;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

}
