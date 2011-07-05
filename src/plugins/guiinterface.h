#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QMainWindow>

namespace OpenCOR {

class GuiInterface
{
public:
    virtual void setupUi(QMainWindow *pMainWindow) = 0;
};

}

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR.GuiInterface")

#endif
