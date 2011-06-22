#ifndef VERSIONMANAGERPLUGIN_H
#define VERSIONMANAGERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

class VersionManagerPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
