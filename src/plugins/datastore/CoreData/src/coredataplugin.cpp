//==============================================================================
// CoreStore plugin
//==============================================================================

#include "coredataplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CoreData {

//==============================================================================

PLUGININFO_FUNC CoreDataPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("the core data store plugin."));

    return new PluginInfo(PluginInfo::Datastore, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace CoreData
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
