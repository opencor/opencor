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

    descriptions.insert("en", QString::fromUtf8("A plugin to access CVODE, IDA and KINSOL from the <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a> library"));
    descriptions.insert("fr", QString::fromUtf8("Une extension pour accéder à CVODE, IDA et KINSOL de la librairie <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a>"));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::General,
                          PluginInfo::ThirdParty,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SUNDIALS
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
