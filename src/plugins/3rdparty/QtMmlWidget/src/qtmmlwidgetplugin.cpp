#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>");

    return PluginInfo(PluginInfo::Gui,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

} }
