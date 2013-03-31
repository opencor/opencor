//==============================================================================
// CellMLModelRepository plugin
//==============================================================================

#include "cellmlmodelrepositoryplugin.h"
#include "cellmlmodelrepositorywindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepository {

//==============================================================================

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin to access the <a href=\"http://models.cellml.org/\">CellML Model Repository</a>"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder au <a href=\"http://models.cellml.org/\">Répertoire de Modèles CellML</a>"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Organisation,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

void CellMLModelRepositoryPlugin::initialize()
{
    // Create an action to show/hide our CellML Model Repository window

    mCellmlModelRepositoryAction = newAction(mMainWindow, true);

    // Create our CellML Model Repository window

    mCellmlModelRepositoryWindow = new CellmlModelRepositoryWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea,
                            mCellmlModelRepositoryWindow,
                            GuiWindowSettings::Organisation,
                            mCellmlModelRepositoryAction);
}

//==============================================================================

void CellMLModelRepositoryPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our CellML Model Repository window settings

    loadWindowSettings(pSettings, mCellmlModelRepositoryWindow);
}

//==============================================================================

void CellMLModelRepositoryPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our CellML Model Repository window settings

    saveWindowSettings(pSettings, mCellmlModelRepositoryWindow);
}

//==============================================================================

void CellMLModelRepositoryPlugin::retranslateUi()
{
    // Retranslate our CellML Model Repository action

    retranslateAction(mCellmlModelRepositoryAction,
                      tr("CellML Model Repository"),
                      tr("Show/hide the CellML Model Repository window"));

    // Retranslate our CellML Model Repository window

    mCellmlModelRepositoryWindow->retranslateUi();
}

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
