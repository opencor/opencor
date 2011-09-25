#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include "guiinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo();

class HelpWindow;

class HelpPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit HelpPlugin();

    virtual void initialize();
    virtual void finalize();

protected:
    virtual void retranslateUi();

private:
    QAction *mHelpAction;

    HelpWindow *mHelpWindow;
};

} }

#endif
