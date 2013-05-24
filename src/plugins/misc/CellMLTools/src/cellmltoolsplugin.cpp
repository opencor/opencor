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

    mToolsCellmlConversionAction = newAction(mMainWindow);

    // Some connections to handle our different File actions

    connect(mToolsCellmlConversionAction, SIGNAL(triggered(bool)),
            this, SLOT(cellmlConversion()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools, mToolsCellmlConversionAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::Tools);
}

//==============================================================================

void CellMLToolsPlugin::retranslateUi()
{
    // Retranslate our different Tools actions

    retranslateAction(mToolsCellmlConversionAction, tr("CellML conversion..."), tr("Convert a CellML file from one version to another"));
}

//==============================================================================

void CellMLToolsPlugin::cellmlConversion()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
