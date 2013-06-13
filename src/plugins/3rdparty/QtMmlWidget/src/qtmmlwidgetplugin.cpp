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

    descriptions.insert("en", QString::fromUtf8("a plugin to access <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>."));
    descriptions.insert("fr", QString::fromUtf8("une extension pour accéder à <a href=\"ftp://ftp.qt.nokia.com/qt/solutions/lgpl/\">QtMmlWidget</a>."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::ThirdParty,
                          false,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace QtMmlWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
