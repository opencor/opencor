#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList());
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

} }
