#ifndef CELLMLPLUGIN_H
#define CELLMLPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace CellML {

PLUGININFO_FUNC CellMLPluginInfo();

class CellMLPlugin : public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    virtual void initialize(const QList<Plugin *> &);
};

} }

#endif
