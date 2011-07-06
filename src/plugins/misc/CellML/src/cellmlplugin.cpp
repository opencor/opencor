#include "cellmlplugin.h"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo()
{
    return PluginInfo(PluginInfo::General,
                      QStringList(),
                      false);
}

void CellMLPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void CellMLPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(CellML, CellMLPlugin)

} }
