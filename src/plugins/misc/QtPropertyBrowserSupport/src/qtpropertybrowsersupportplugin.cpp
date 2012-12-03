//==============================================================================
// QtPropertyBrowserSupport plugin
//==============================================================================

#include "qtpropertybrowsersupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowserSupport {

//==============================================================================

PLUGININFO_FUNC QtPropertyBrowserSupportPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to support <a href=\"http://qt.gitorious.org/qt-solutions/qt-solutions/trees/master/qtpropertybrowser/\">QtPropertyBrowser</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour supporter <a href=\"http://qt.gitorious.org/qt-solutions/qt-solutions/trees/master/qtpropertybrowser/\">QtPropertyBrowser</a>"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Miscellaneous,
                          false,
                          QStringList() << "Core" << "QtPropertyBrowser",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QtPropertyBrowserSupport, QtPropertyBrowserSupportPlugin)

//==============================================================================

}   // namespace QtPropertyBrowserSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
