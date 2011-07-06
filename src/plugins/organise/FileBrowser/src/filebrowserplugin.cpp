#include "filebrowserplugin.h"

namespace OpenCOR {
namespace FileBrowser {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core");
}

void FileBrowserPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void FileBrowserPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void FileBrowserPlugin::initializeGui(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

void FileBrowserPlugin::finalizeGui()
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

} }
