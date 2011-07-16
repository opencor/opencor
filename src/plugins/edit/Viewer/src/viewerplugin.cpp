#include "viewerplugin.h"

namespace OpenCOR {
namespace Viewer {

PLUGININFO_FUNC ViewerPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to graphically visualise mathematical equations");
    descriptions.insert("fr", "Une extension pour visualiser graphiquement des équations mathématiques");

    return PluginInfo(PluginInterface::V001, PluginInfo::Gui, true,
                      QStringList() << "Core" << "QtMmlWidget",
                      descriptions);
}

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

ViewerPlugin::ViewerPlugin() :
    GuiInterface("Viewer")
{
}

} }
