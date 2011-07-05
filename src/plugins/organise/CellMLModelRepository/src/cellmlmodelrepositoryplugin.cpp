#include "cellmlmodelrepositoryplugin.h"

#include <QMessageBox>

namespace OpenCOR {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void CellMLModelRepositoryPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...

QMessageBox::information(0, "CellMLModelRepository plugin", "CellMLModelRepository plugin...");
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

}
