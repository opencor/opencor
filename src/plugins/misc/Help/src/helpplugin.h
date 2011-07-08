#ifndef HELPPLUGIN_H
#define HELPPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo();

class HelpPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
