#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    PluginInfo info;

    info.type         = PluginInfo::Gui;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
