#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "mainwindow.h"
#include "plugininterface.h"

namespace OpenCOR {

class GuiInterface : public PluginInterface
{
public:
    virtual void initialize(MainWindow *);
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
