#include "cellmlmodelrepositoryplugin.h"
#include "cellmlmodelrepositorywindow.h"

#include <QMainWindow>

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to access the <a href=\"http://models.cellml.org/\">CellML Model Repository</a>");
    descriptions.insert("fr", "Une extension pour accéder au <a href=\"http://models.cellml.org/\">Répertoire de Modèles CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Organisation,
                      true,
                      QStringList() << "Core",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellMLModelRepository, CellMLModelRepositoryPlugin)

void CellMLModelRepositoryPlugin::initialize()
{
    // Create our CellML Model Repository window

    mCellmlModelRepositoryWindow = new CellmlModelRepositoryWindow(mMainWindow);
}

void CellMLModelRepositoryPlugin::loadSettings(QSettings *pSettings,
                                               const bool &pNeedDefaultSettings)
{
    // Retrieve our CellML Model Repository window settings

    loadWindowSettings(pSettings, pNeedDefaultSettings, Qt::LeftDockWidgetArea,
                       mCellmlModelRepositoryWindow);
}

void CellMLModelRepositoryPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML Model Repository window settings

    saveWindowSettings(pSettings, mCellmlModelRepositoryWindow);
}

void CellMLModelRepositoryPlugin::retranslateUi()
{
    // Retranslate our CellML Model Repository window

    mCellmlModelRepositoryWindow->retranslateUi();
}

} }
