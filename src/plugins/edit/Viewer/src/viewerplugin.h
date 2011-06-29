#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC ViewerPluginInfo();

class ViewerPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
