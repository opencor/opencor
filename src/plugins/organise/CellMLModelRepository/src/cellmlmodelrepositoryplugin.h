#ifndef CELLMLMODELREPOSITORYPLUGIN_H
#define CELLMLMODELREPOSITORYPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo();

class CellMLModelRepositoryPlugin : public QObject,
                                    public PluginInterface,
                                    public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
