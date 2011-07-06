#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QMainWindow>

namespace OpenCOR {

class GuiInterface
{
public:
    virtual void initializeGui(QMainWindow *);
    virtual void finalizeGui();
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
