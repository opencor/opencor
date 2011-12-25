//==============================================================================
// CVODE plugin
//==============================================================================

#include "cvodeplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CVODE {

//==============================================================================

PLUGININFO_FUNC CVODEPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::ThirdParty,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CVODE, CVODEPlugin)

//==============================================================================

}   // namespace CVODE
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
