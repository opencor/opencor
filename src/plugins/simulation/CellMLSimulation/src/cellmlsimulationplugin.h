#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

#include "plugininfo.h"

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public QObject
{
    Q_OBJECT
};

} }

#endif
