#include "viewerplugin.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core" << "QtMmlWidget",
                       true);
}

void ViewerPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void ViewerPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void ViewerPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void ViewerPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

} }
