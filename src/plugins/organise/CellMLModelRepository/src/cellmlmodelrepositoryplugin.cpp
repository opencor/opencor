#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       QObject::tr("a plugin which provides you with a means to access the <a href=\"http://models.cellml.org/\">CellML Model Repository</a>"));
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

} }
