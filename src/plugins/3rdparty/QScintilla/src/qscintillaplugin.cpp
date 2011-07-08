#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList(),
                      QObject::tr("a plugin which provides other plugins with an interface to the <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a> third-party library"));
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
