#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       true);
}

void CellMLModelRepositoryPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void CellMLModelRepositoryPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void CellMLModelRepositoryPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void CellMLModelRepositoryPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

} }
