#ifndef VERSIONMANAGERPLUGIN_H
#define VERSIONMANAGERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace VersionManager {

PLUGININFO_FUNC VersionManagerPluginInfo();

class VersionManagerPlugin : public QObject,
                             public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    // Plugin interface

    virtual void initialize();
    virtual void finalize();
};

} }

#endif
