//==============================================================================
// Viewer plugin
//==============================================================================

#include "viewerplugin.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

PLUGININFO_FUNC ViewerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to graphically visualise various modelling concepts (e.g. mathematical equations)");
    descriptions.insert("fr", "Une extension pour visualiser graphiquement différents concepts de modélisation (par exemple des équations mathématiques)");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Miscellaneous,
                      true,
                      QStringList() << "CoreEditing" << "QtMmlWidget",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
