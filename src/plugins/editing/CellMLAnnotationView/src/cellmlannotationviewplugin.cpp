//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to annotate CellML files");
    descriptions.insert("fr", "Une extension pour annoter des fichiers CellML");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      true,
                      QStringList() << "CoreCellMLEditing" << "RICORDOSupport",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLAnnotationView, CellMLAnnotationViewPlugin)

//==============================================================================

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic raw CellML view widget

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // We are dealing with a CellML file, so create and return a CellML
    // annotation view widget

    return new CellmlAnnotationViewWidget(pFileName, mMainWindow);
}

//==============================================================================

QString CellMLAnnotationViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("CellML Annotation");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
