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

    descriptions.insert("en", "A plugin to access CVODE and IDA from the <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a> library");
    descriptions.insert("fr", "Une extension pour acc√©der √  CVODE et IDA de la librairie <a href=\"https://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a>");

    return new PluginInfo(PluginInfo::FormatVersion001,
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
