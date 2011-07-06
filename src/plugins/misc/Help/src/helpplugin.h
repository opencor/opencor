#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo();

class HelpPlugin : public QObject,
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
