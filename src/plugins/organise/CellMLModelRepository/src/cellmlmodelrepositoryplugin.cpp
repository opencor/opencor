#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

}
