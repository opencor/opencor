//==============================================================================
// IDA plugin
//==============================================================================

#include "idaplugin.h"

//==============================================================================

namespace OpenCOR {
namespace IDA {

//==============================================================================

PLUGININFO_FUNC IDAPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::ThirdParty,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(IDA, IDAPlugin)

//==============================================================================

}   // namespace IDA
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
