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

void HelpPlugin::initialize()
{
//---GRY--- TO BE DONE...
}

void HelpPlugin::finalize()
{
//---GRY--- TO BE DONE...
}

void HelpPlugin::setupUi(QMainWindow *pMainWindow)
{
//---GRY--- TO BE DONE...
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

} }
