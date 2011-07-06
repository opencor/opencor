#include "helpplugin.h"

#include <QMessageBox>

namespace OpenCOR {
namespace Help {

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

QMessageBox::information(0, "Help plugin", "Help plugin...");
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

} }
