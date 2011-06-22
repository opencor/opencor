#ifndef VERSIONMANAGERPLUGIN_H
#define VERSIONMANAGERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

class VersionManagerPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
