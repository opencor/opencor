#include "qtmmlwidgetplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList();
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

}
