//==============================================================================
// CoreStore plugin
//==============================================================================

#include "csvstoreplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CsvStore {

//==============================================================================

PLUGININFO_FUNC CsvStorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the CSV data store interface."));
    descriptions.insert("fr", QString::fromUtf8("the CSV data store interface."));

    return new PluginInfo(PluginInfo::Datastore, true, false,
                          QStringList() << "CoreData",
                          descriptions);
}

//==============================================================================

}   // namespace CsvStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
