#ifndef FILEORGANISERPLUGIN_H
#define FILEORGANISERPLUGIN_H

#include "guiinterface.h"
#include "plugininterface.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo();

class FileOrganiserPlugin : public QObject,
                            public PluginInterface,
                            public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
};

} }

#endif
