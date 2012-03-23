//==============================================================================
// RawCellMLView plugin
//==============================================================================

#include "cellmlsupportplugin.h"
#include "rawcellmlviewplugin.h"
#include "rawcellmlviewwidget.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

PLUGININFO_FUNC RawCellMLViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to edit <a href=\"http://www.cellml.org/\">CellML</a> files using an XML editor");
    descriptions.insert("fr", "Une extension pour éditer des fichiers <a href=\"http://www.cellml.org/\">CellML</a> à l'aide d'un éditeur XML");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      true,
                      QStringList() << "CoreCellMLEditing" << "QScintillaSupport",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(RawCellMLView, RawCellMLViewPlugin)

//==============================================================================

RawCellMLViewPlugin::RawCellMLViewPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

QWidget * RawCellMLViewPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (!CellMLSupport::isCellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return GuiInterface::newViewWidget(pFileName);

    // Create and return a raw CellML view widget

    return new RawCellmlViewWidget(pFileName, mMainWindow);
}

//==============================================================================

QString RawCellMLViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("Raw CellML");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
