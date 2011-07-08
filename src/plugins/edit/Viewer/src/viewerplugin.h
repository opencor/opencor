#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo();

class ViewerPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
