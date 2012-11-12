//==============================================================================
// QtMmlWidget plugin
//==============================================================================

#include "qtmmlwidgetplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QtMmlWidget {

//==============================================================================

PLUGININFO_FUNC QtMmlWidgetPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QtMmlWidget, QtMmlWidgetPlugin)

//==============================================================================

}   // namespace QtMmlWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
