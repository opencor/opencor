#include "fileorganiserplugin.h"

#include <QMessageBox>

namespace OpenCOR {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void FileOrganiserPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...

//QMessageBox::information(0, "FileOrganiser plugin", "FileOrganiser plugin...");
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

}
