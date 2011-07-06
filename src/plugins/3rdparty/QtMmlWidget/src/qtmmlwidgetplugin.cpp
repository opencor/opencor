#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::Gui;
    info.dependencies = QStringList();

    return info;
}

void QtMmlWidgetPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void QtMmlWidgetPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

} }
