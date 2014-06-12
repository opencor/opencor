//==============================================================================
// CoreStore plugin
//==============================================================================

#include "coredatastoreplugin.h"

//==============================================================================

namespace OpenCOR {
namespace CoreDatastore {

//==============================================================================

PLUGININFO_FUNC CoreDatastorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core data store plugin."));
    descriptions.insert("fr", QString::fromUtf8("the core data store plugin."));

    return new PluginInfo(PluginInfo::Datastore, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace CoreDatastore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
