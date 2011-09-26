#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public QObject, public GuiInterface,
                               public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    explicit CellMLSimulationPlugin();

protected:
    virtual void retranslateUi();
};

} }

#endif
