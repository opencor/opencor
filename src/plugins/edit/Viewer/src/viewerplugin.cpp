#include "viewerplugin.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core" << "QtMmlWidget",
                       QObject::tr("A plugin to graphically visualise mathematical equations"));
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

} }
