#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo();

class ViewerPlugin : public QObject,
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
