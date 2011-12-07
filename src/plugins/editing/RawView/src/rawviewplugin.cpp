//==============================================================================
// RawView plugin
//==============================================================================

#include "commonwidget.h"
#include "qscintilla.h"
#include "rawviewplugin.h"

//==============================================================================

#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

PLUGININFO_FUNC RawViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to edit files using a text editor");
    descriptions.insert("fr", "Une extension pour éditer des fichiers à l'aide d'un éditeur de texte");

    return PluginInfo(PluginInfo::V001,
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

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

//==============================================================================

QWidget * RawViewPlugin::newViewWidget(const QString &pFileName)
{
    // Create, set up and return a raw Scintilla editor

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        // For some reason, the file couldn't be opened, so...

        return GuiInterface::newViewWidget(pFileName);

    // The file was properly opened, so create a Scintilla editor

    QsciScintilla *res = new QScintillaSupport::QScintilla(QTextStream(&file).readAll(),
                                                           !(QFileInfo(pFileName).isWritable()),
                                                           0, mMainWindow);

    // We are done with the file, so close it

    file.close();

    // Our raw Scintilla editor is now ready, so...

    return res;
}

//==============================================================================

QString RawViewPlugin::viewName(const int &pViewIndex)
{
    // We have only one view, so return its name otherwise call the GuiInterface
    // implementation of viewName

    switch (pViewIndex) {
    case 0:
        return tr("Raw");
    default:
        return GuiInterface::viewName(pViewIndex);
    }
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
