#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo();

class CorePlugin : public QObject,
                   public PluginInterface,
                   public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    // Plugin interface

    virtual void initialize();
    virtual void finalize();

    // GUI interface

    virtual void initializeGui(QMainWindow *pMainWindow);
    virtual void finalizeGui();
};

} }

#endif
