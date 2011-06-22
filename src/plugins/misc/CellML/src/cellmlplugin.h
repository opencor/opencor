#ifndef CELLMLPLUGIN_H
#define CELLMLPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

class CellmlPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
