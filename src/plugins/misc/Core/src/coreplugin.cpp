#include "coreplugin.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::General;
    res.dependencies = QStringList();

    return res;
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

} }
