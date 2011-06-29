#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "Core";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

}
