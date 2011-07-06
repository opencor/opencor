#ifndef VERSIONMANAGERPLUGIN_H
#define VERSIONMANAGERPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace VersionManager {

PLUGININFO_FUNC VersionManagerPluginInfo();

class VersionManagerPlugin : public QObject,
                             public PluginInterface,
                             public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
