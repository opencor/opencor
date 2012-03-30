//==============================================================================
// CellMLAnnotationView plugin
//==============================================================================

#include "cellmlannotationviewplugin.h"
#include "cellmlannotationviewwidget.h"
#include "cellmlfilemanager.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QSettings>

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

CellMLAnnotationViewPlugin::CellMLAnnotationViewPlugin() :
    mTreeViewWidth(0),
    mCellmlAnnotationWidth(0)
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

static const QString SettingsCellmlAnnotationWidget = "CellmlAnnotationWidget";
static const QString SettingsTreeviewWidth          = "TreeviewWidth";
static const QString SettingsCellmlAnnotationWidth  = "CellmlAnnotationWidth";

//==============================================================================

void CellMLAnnotationViewPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve the settings of the tree view and CellML annotation's width
    // Note #1: we would normally do this in CellmlAnnotationViewWidget, but
    //          we have one instance of it per CellML file and we want to share
    //          some information between the different instances, so we do it
    //          here...
    // Note #1: the tree view's default width is 15% of the desktop's width
    //          while of the CellML annotation is as big as it can be...
    // Note #2: because the CellML annotation's default width is much bigger
    //          than that of our tree view, the tree view's default width will
    //          effectively be less than 15% of the desktop's width, but that
    //          doesn't matter at all...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        mTreeViewWidth = pSettings->value(SettingsTreeviewWidth,
                                          0.15*qApp->desktop()->screenGeometry().width()).toInt();
        mCellmlAnnotationWidth = pSettings->value(SettingsCellmlAnnotationWidth,
                                                  qApp->desktop()->screenGeometry().width()).toInt();
    pSettings->endGroup();
}

//==============================================================================

void CellMLAnnotationViewPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the tree view's and CellML annotation's width
    // Note: we must also keep track of the CellML annotation's width because
    //       when loading our settings (see above), the widget doesn't yet have
    //       a 'proper' width, so we couldn't simply assume that the Cellml
    //       annotation's initial width is this widget's width minus the tree
    //       view's initial width, so...

    pSettings->beginGroup(SettingsCellmlAnnotationWidget);
        pSettings->setValue(SettingsTreeviewWidth, mTreeViewWidth);
        pSettings->setValue(SettingsCellmlAnnotationWidth, mCellmlAnnotationWidth);
    pSettings->endGroup();
}

//==============================================================================

QWidget * CellMLAnnotationViewPlugin::newViewWidget(const QString &pFileName)
{
    // Check that we are dealing with a CellML file and, if so, return our
    // generic raw CellML view widget

    if (!CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName))
        // We are not dealing with a CellML file, so...

        return 0;

    // We are dealing with a CellML file, so create, keep track of and return a
    // CellML annotation view widget

    CellmlAnnotationViewWidget *widget = new CellmlAnnotationViewWidget(mMainWindow, pFileName);

    mWidgets.append(widget);

    return widget;
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
