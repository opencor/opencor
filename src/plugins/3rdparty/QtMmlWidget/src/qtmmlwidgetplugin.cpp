#include "qtmmlwidgetplugin.h"

namespace OpenCOR {
namespace QtMmlWidget {

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::Gui,
                      PluginInfo::ThirdParty,
                      false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

} }
