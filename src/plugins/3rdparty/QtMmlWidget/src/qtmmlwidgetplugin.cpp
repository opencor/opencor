#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList(),
                      QObject::tr("a plugin which provides other plugins with an interface to the <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a> third-party library"));
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

} }
