#include "viewerplugin.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core" << "QtMmlWidget");
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

} }
