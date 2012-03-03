//==============================================================================
// SUNDIALS plugin
//==============================================================================

#include "sundialsplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SUNDIALS {

//==============================================================================

PLUGININFO_FUNC SUNDIALSPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to use <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a>");
    descriptions.insert("fr", "Une extension pour utiliser <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::General,
                      PluginInfo::ThirdParty,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(SUNDIALS, SUNDIALSPlugin)

//==============================================================================

}   // namespace SUNDIALS
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
