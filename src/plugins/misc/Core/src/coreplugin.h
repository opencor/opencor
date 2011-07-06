#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo();

class CorePlugin : public QObject,
                   public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    // Plugin interface

    virtual void initialize();
    virtual void finalize();
};

} }

#endif
