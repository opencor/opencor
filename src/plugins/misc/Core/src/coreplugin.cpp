#include "coreplugin.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList(),
                      false);
}

void CorePlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void CorePlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void CorePlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void CorePlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

} }
