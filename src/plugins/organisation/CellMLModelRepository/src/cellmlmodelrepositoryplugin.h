#ifndef CELLMLMODELREPOSITORYPLUGIN_H
#define CELLMLMODELREPOSITORYPLUGIN_H

#include "guiinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo();

class CellMLModelRepositoryPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CellMLModelRepositoryPlugin();
};

} }

#endif
