#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {

class CorePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
};

}

#endif
