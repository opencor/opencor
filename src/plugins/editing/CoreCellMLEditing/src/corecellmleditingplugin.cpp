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

    descriptions.insert("en", QString::fromUtf8("The core CellML editing plugin"));
    descriptions.insert("fr", QString::fromUtf8("L'extension d'édition CellML de base"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
                          false,
                          QStringList() << "CoreEditing" << "CellMLSupport",
                          descriptions);
}

//==============================================================================

void CoreCellMLEditingPlugin::initialize()
{
    // Create our different File|New actions

    mFileNewCellml1_0Action = newAction(mMainWindow);
    mFileNewCellml1_1Action = newAction(mMainWindow);

    // Set our settings

/*---GRY--- DISABLED FOR VERSION 0.1...
    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_0Action);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewCellml1_1Action);
*/
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
