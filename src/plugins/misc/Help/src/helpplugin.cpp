#include "helpplugin.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       true);
}

void HelpPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void HelpPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void HelpPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void HelpPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

} }
