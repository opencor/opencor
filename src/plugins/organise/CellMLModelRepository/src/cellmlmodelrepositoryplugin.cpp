#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core");
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

} }
