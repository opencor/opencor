#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include "plugininterface.h"

#include <QMainWindow>

namespace OpenCOR {

class GuiInterface : public PluginInterface
{
public:
    virtual void initialize(QMainWindow *);
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
