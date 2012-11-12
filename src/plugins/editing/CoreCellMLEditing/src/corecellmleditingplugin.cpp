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

    descriptions.insert("en", "The core CellML editing plugin");
    descriptions.insert("fr", "L'extension d'édition CellML de base");

    return new PluginInfo(PluginInfo::FormatVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
                          false,
                          QStringList() << "CoreEditing" << "CellMLSupport",
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

//---GRY--- WE DISABLE THE ABOVE ACTIONS SINCE THEY ARE NOT YET IMPLEMENTED...

mFileNewCellml1_0Action->setEnabled(false);
mFileNewCellml1_1Action->setEnabled(false);

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_0Action);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_1Action);
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

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
