#include "helpplugin.h"

#include <QMessageBox>

namespace OpenCOR {

PLUGININFO_FUNC HelpPluginInfo()
{
    PluginInfo res;

    res.type         = PluginInfo::Gui;
    res.dependencies = QStringList() << "Core";

    return res;
}

void HelpPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...

QMessageBox::information(pMainWindow, "Help plugin", "Help plugin...");
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

}
