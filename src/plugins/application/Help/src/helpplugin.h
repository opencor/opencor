#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo();

class HelpWindow;

class HelpPlugin : public GuiInterface, public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
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
