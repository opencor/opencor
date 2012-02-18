//==============================================================================
// CellMLAnnotation plugin
//==============================================================================

#include "cellmlannotationplugin.h"
#include "cellmlsupportplugin.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotation {

//==============================================================================

PLUGININFO_FUNC CellMLAnnotationPluginInfo()
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

Q_EXPORT_PLUGIN2(CellMLAnnotation, CellMLAnnotationPlugin)

//==============================================================================

CellMLAnnotationPlugin::CellMLAnnotationPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

QWidget * CellMLAnnotationPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file

    if (!CellMLSupport::isCellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return GuiInterface::newViewWidget(pFileName);

    // Create, set up and return a CellML annotation widget

    QWidget *res = new QWidget(mMainWindow);

    res->setStyleSheet("background-color: white;"
                       "background-image: url(':ricordoLogo');"
                       "background-position: center;"
                       "background-repeat: no-repeat;");

    // Our CellML annotation widget is now ready, so...

    return res;
}

//==============================================================================

QString CellMLAnnotationPlugin::viewName(const int &pViewIndex)
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

}   // namespace CellMLAnnotation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
