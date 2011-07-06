#include "cellmlmodelrepositoryplugin.h"

#include <QMessageBox>

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void CellMLModelRepositoryPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void CellMLModelRepositoryPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void CellMLModelRepositoryPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

} }
