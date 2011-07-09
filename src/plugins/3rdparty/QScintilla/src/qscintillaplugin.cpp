#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList(),
                      QObject::tr("A plugin to use <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a>"));
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
