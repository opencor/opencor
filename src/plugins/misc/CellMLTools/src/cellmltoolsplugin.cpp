//==============================================================================
// CellMLTools plugin
//==============================================================================

#include "cellmltoolsplugin.h"

//==============================================================================

#include <QAction>

//==============================================================================

namespace OpenCOR {
namespace CellMLTools {

//==============================================================================

PLUGININFO_FUNC CellMLToolsPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("A plugin which gives access to various <a href=\"http://www.cellml.org/\">CellML</a>-related tools"));
    descriptions.insert("fr", QString::fromUtf8("Une extension qui donne accès à divers outils en rapport avec <a href=\"http://www.cellml.org/\">CellML</a>"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::Miscellaneous,
                          true,
                          QStringList() << "CellMLSupport",
                          descriptions);
}

//==============================================================================

void CellMLToolsPlugin::initialize()
{
    // Create our different Tools actions

    mCellmlVersionConverterAction = newAction(mMainWindow);

    // Some connections to handle our different File actions

    connect(mCellmlVersionConverterAction, SIGNAL(triggered(bool)),
            this, SLOT(cellmlVersionConverter()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools, mCellmlVersionConverterAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools);
}

//==============================================================================

void CellMLToolsPlugin::retranslateUi()
{
    // Retranslate our different Tools actions

    retranslateAction(mCellmlVersionConverterAction, tr("CellML Version Converter..."), tr("Convert a CellML file to another CellML version"));
}

//==============================================================================

void CellMLToolsPlugin::cellmlVersionConverter()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
