#ifndef CORESIMULATIONPLUGIN_H
#define CORESIMULATIONPLUGIN_H

#include "coreinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CoreSimulation {

PLUGININFO_FUNC CoreSimulationPluginInfo();

class CoreSimulationPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
