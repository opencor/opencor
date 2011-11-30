//==============================================================================
// RawCellMLView plugin
//==============================================================================

#include "commonwidget.h"
#include "rawcellmlviewplugin.h"

//==============================================================================

#include <QFile>
#include <QMainWindow>
#include <QTextStream>

//==============================================================================

#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerxml.h"

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
                      QStringList() << "CoreCellMLEditing" << "QScintilla",
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
    // Create, set up and return a raw CellML Scintilla editor

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

    // Associate an XML (i.e. raw CellML) lexer to our Scintilla editor and set
    // a default font family and size to it

    QsciLexerXML *xmlLexer = new QsciLexerXML(res);

    QFont defaultFont = QFont(OpenCOR::Core::DefaultFontFamily,
                              OpenCOR::Core::DefaultFontSize);

    xmlLexer->setDefaultFont(defaultFont);
    xmlLexer->setFont(defaultFont);

    res->setLexer(xmlLexer);

    // Specify the type of tree folding to used

    res->setFolding(QsciScintilla::BoxedTreeFoldStyle);

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
