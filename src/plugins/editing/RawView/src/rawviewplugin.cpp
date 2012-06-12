//==============================================================================
// RawView plugin
//==============================================================================

#include "rawviewplugin.h"
#include "rawviewwidget.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

PLUGININFO_FUNC RawViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to edit files using a text editor");
    descriptions.insert("fr", "Une extension pour éditer des fichiers à l'aide d'un éditeur de texte");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::Gui,
                          PluginInfo::Editing,
                          true,
                          QStringList() << "CoreEditing" << "QScintillaSupport",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(RawView, RawViewPlugin)

//==============================================================================

RawViewPlugin::RawViewPlugin()
{
    // Set our settings

    mGuiSettings->setView(GuiViewSettings::Editing);
}

//==============================================================================

QWidget * RawViewPlugin::newViewWidget(const QString &pFileName)
{
    // Create and return a raw view widget

    return new RawViewWidget(mMainWindow, pFileName);
}

//==============================================================================

bool RawViewPlugin::deleteViewWidget(const QString &pFileName)
{
    // Our parent's version keeps track of our view widgets, so call it...

    if (!GuiInterface::deleteViewWidget(pFileName))
        return false;

//---GRY--- TO BE DONE...

    return false;
}

//==============================================================================

QString RawViewPlugin::viewName()
{
    // Return our raw view's name

    return tr("Raw");
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
