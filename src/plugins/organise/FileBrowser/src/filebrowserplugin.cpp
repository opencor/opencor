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

void FileBrowserPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...

QMessageBox::information(0, "FileBrowser plugin", "FileBrowser plugin...");
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

} }
