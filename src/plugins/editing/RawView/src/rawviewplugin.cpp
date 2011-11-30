//==============================================================================
// RawView plugin
//==============================================================================

#include "commonwidget.h"
#include "rawviewplugin.h"

//==============================================================================

#include <QFile>
#include <QMainWindow>
#include <QTextStream>

//==============================================================================

#include "Qsci/qsciscintilla.h"

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
                      QStringList() << "CoreEditing" << "QScintilla",
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

    QsciScintilla *res = new QsciScintilla(mMainWindow);

    // Remove the frame around our Scintilla editor

    res->setFrameShape(QFrame::NoFrame);

    // Remove the margin in our Scintilla editor

    res->setMarginWidth(1, 0);

    // Specify a default font family and size for our Scintilla editor

    res->setFont(QFont(OpenCOR::Core::DefaultFontFamily,
                       OpenCOR::Core::DefaultFontSize));

    // Set the text in the Scintilla editor to the contents of the file and
    // specify whether the editor should be read-only

    res->setText(QTextStream(&file).readAll());
    res->setReadOnly(!(QFile::permissions(pFileName) & QFile::WriteUser));

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
