#include "filebrowserplugin.h"

#include <QMessageBox>

namespace OpenCOR {
namespace FileBrowser {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void FileBrowserPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void FileBrowserPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void FileBrowserPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

} }
