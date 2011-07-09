#include "coreplugin.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList(),
                      QObject::tr("The core plugin for OpenCOR"));
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

} }
