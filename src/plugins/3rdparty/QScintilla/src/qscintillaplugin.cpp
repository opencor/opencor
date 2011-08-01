#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>");

    return PluginInfo(PluginInterface::V001, PluginInfo::Gui,
                      PluginInfo::ThirdParty, false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
