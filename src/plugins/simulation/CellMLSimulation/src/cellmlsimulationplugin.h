#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

#include "plugininterface.h"

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
