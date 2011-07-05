#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC ViewerPluginInfo();

class ViewerPlugin : public QObject,
                     public PluginInterface,
                     public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    // GuiInterface

    virtual void setupUi(QMainWindow *pMainWindow);
};

}

#endif
