#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList());
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
