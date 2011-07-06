#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList(),
                      false);
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
