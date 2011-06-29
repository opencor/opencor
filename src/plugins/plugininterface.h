#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include <QStringList>

namespace OpenCOR {

#ifdef Q_WS_WIN
    #define PLUGININFO_FUNC extern "C" __declspec(dllexport) PluginInfo
#else
    #define PLUGININFO_FUNC extern "C" PluginInfo
#endif

enum PluginType
{
    Both,
    Console,
    Gui
};

struct PluginInfo
{
    QStringList dependencies;
    PluginType type;
};

class PluginInterface : public QObject
{
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
