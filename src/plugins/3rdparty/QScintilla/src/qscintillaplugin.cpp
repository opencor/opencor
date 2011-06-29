#include "qscintillaplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    PluginInfo info;

    info.dependencies = QStringList() << "";
    info.type         = Gui;

    return info;
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

}
