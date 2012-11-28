//==============================================================================
// QtPropertyBrowser plugin
//==============================================================================

#include "qtpropertybrowserplugin.h"

//==============================================================================

namespace OpenCOR {
namespace QtPropertyBrowser {

//==============================================================================

PLUGININFO_FUNC QtPropertyBrowserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to access <a href=\"http://qt.gitorious.org/qt-solutions/qt-solutions/trees/master/qtpropertybrowser/\">QtPropertyBrowser</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à <a href=\"http://qt.gitorious.org/qt-solutions/qt-solutions/trees/master/qtpropertybrowser/\">QtPropertyBrowser</a>"));

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(QtPropertyBrowser, QtPropertyBrowserPlugin)

//==============================================================================

}   // namespace QtPropertyBrowser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
