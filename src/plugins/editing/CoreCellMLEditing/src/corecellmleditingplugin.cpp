#include "corecellmleditingplugin.h"

namespace OpenCOR {
namespace CoreCellMLEditing {

PLUGININFO_FUNC CoreCellMLEditingPluginInfo()
{
    PluginInfoDescriptions descriptions;

    descriptions.insert("en", "The core CellML editing plugin for OpenCOR");
    descriptions.insert("fr", "L'extension d'édition CellML de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      false,
                      QStringList() << "CoreEditing" << "CellML",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CoreCellMLEditing, CoreCellMLEditingPlugin)

} }
