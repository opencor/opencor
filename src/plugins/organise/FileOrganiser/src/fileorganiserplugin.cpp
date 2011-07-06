#include "fileorganiserplugin.h"

#include <QMessageBox>

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void FileOrganiserPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void FileOrganiserPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void FileOrganiserPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

} }
