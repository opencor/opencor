#include "qscintillaplugin.h"

namespace OpenCOR {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    PluginInfo info;

    info.type         = Gui;
    info.dependencies = QStringList();

    return info;
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

}
