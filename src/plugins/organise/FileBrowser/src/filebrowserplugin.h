#ifndef FILEBROWSERPLUGIN_H
#define FILEBROWSERPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {

PLUGININFO_FUNC FileBrowserPluginInfo();

class FileBrowserPlugin : public QObject,
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
