#include "cellmlmodelrepositoryplugin.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to access the <a href=\"http://models.cellml.org/\">CellML Model Repository</a>");
    descriptions.insert("fr", "Une extension pour accéder au <a href=\"http://models.cellml.org/\">Répertoire de Modèles CellML</a>");

    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

} }
