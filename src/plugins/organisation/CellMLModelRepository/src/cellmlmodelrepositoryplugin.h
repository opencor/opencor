#ifndef CELLMLMODELREPOSITORYPLUGIN_H
#define CELLMLMODELREPOSITORYPLUGIN_H

#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CellMLModelRepository {

PLUGININFO_FUNC CellMLModelRepositoryPluginInfo();

class CellMLModelRepositoryPlugin : public QObject, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
};

} }

#endif
