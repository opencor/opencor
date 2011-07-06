#include "fileorganiserplugin.h"

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       true);
}

void FileOrganiserPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void FileOrganiserPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void FileOrganiserPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void FileOrganiserPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

} }
