//==============================================================================
// CoreCellMLEditing plugin
//==============================================================================

#include "corecellmleditingplugin.h"

//==============================================================================

#include <QAction>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

PLUGININFO_FUNC CoreCellMLEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core CellML editing plugin for OpenCOR");
    descriptions.insert("fr", "L'extension d'édition CellML de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      false,
                      QStringList() << "CoreEditing" << "CellMLModel",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreCellMLEditing, CoreCellMLEditingPlugin)

//==============================================================================

void CoreCellMLEditingPlugin::initialize()
{
    // Create our different File|New actions

    mFileNewCellml1_0Action = newAction(mMainWindow);
    mFileNewCellml1_1Action = newAction(mMainWindow);

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_0Action);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_1Action);

    // Initialise the enabled state of our various actions

    updateActions();
}

//==============================================================================

void CoreCellMLEditingPlugin::retranslateUi()
{
    // Retranslate our different File|New actions

    retranslateAction(mFileNewCellml1_0Action, tr("CellML 1.0 File"),
                      tr("Create a new CellML 1.0 file"));
    retranslateAction(mFileNewCellml1_1Action, tr("CellML 1.1 File"),
                      tr("Create a new CellML 1.1 file"));
}

//==============================================================================

void CoreCellMLEditingPlugin::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    mFileNewCellml1_0Action->setEnabled(false);
    mFileNewCellml1_1Action->setEnabled(false);
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
