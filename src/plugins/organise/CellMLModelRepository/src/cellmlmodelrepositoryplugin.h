#ifndef CELLMLMODELREPOSITORYPLUGIN_H
#define CELLMLMODELREPOSITORYPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo();

class CellMLModelRepositoryPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
