//==============================================================================
// RawCellMLView plugin
//==============================================================================

#include "cellmlfilemanager.h"
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

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
                          true,
                          QStringList() << "CoreCellMLEditing" << "QScintillaSupport" << "Viewer",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(RawCellMLView, RawCellMLViewPlugin)

//==============================================================================

RawCellMLViewPlugin::RawCellMLViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing);
}

//==============================================================================

void RawCellMLViewPlugin::initialize()
{
    // Create our generic raw CellML view widget

    mViewWidget = new RawCellmlViewWidget(mMainWindow);

    // Hide our generic raw CellML view widget since it may not initially be
    // shown in our central widget

    mViewWidget->setVisible(false);
}

//==============================================================================

void RawCellMLViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our generic raw CellML view widget settings

    loadViewSettings(pSettings, mViewWidget);
}

//==============================================================================

void RawCellMLViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Retrieve our generic raw CellML view widget settings

    saveViewSettings(pSettings, mViewWidget);
}

//==============================================================================

QWidget * RawCellMLViewPlugin::viewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic raw CellML view widget

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // We are dealing with a CellML file, so update our generic raw CellML view
    // widget using the passed CellML file

    mViewWidget->initialize(pFileName);

    // Our generic raw CellML view widget is now ready, so...

    return mViewWidget;
}

//==============================================================================

bool RawCellMLViewPlugin::deleteViewWidget(const QString &pFileName)
{
    // First, call our parent's version

    if (!GuiInterface::deleteViewWidget(pFileName))
        return false;
}

//==============================================================================

QString RawCellMLViewPlugin::viewName()
{
    // Return our raw CellML view's name

    return tr("Raw CellML");
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
