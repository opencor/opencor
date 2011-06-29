#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC HelpPluginInfo();

class HelpPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
