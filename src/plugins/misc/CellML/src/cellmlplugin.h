#ifndef CELLMLPLUGIN_H
#define CELLMLPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC CellmlPluginInfo();

class CellMLPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
