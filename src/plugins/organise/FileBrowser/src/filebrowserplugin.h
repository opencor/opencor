#ifndef FILEBROWSERPLUGIN_H
#define FILEBROWSERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC FileBrowserPluginInfo();

class FileBrowserPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
