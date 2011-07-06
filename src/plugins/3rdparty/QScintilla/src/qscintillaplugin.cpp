#include "qscintillaplugin.h"

namespace OpenCOR {
namespace QScintilla {

PLUGININFO_FUNC QScintillaPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                      QStringList());
}

void QScintillaPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void QScintillaPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(QScintilla, QScintillaPlugin)

} }
