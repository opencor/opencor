#ifndef FILEORGANISERPLUGIN_H
#define FILEORGANISERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC FileOrganiserPluginInfo();

class FileOrganiserPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
