#include "rawviewplugin.h"

#include <QFile>
#include <QMainWindow>
#include <QTextEdit>

namespace OpenCOR {
namespace RawView {

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

Q_EXPORT_PLUGIN2(RawView, RawViewPlugin)

RawViewPlugin::RawViewPlugin()
{
    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Editing, 0);
}

QWidget * RawViewPlugin::newViewWidget(const QString &pFileName)
{
    // Create, set up and return a raw Scintilla editor

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return GuiInterface::newViewWidget(pFileName);

    QTextEdit *res = new QTextEdit(mMainWindow);

    res->setFrameStyle(QFrame::NoFrame);
    res->setFontPointSize(11);
    res->setPlainText(file.readAll());

    return res;
}

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

} }
