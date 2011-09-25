#ifndef FILEORGANISERPLUGIN_H
#define FILEORGANISERPLUGIN_H

#include "guiinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo();

class FileOrganiserPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit FileOrganiserPlugin();
};

} }

#endif
