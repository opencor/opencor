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

struct PluginInfo
{
    enum PluginType
    {
        Undefined,
        General,
        Console,
        Gui
    };

    PluginType type;
    QStringList dependencies;
};

class PluginInterface
{
public:
    virtual void initialize() = 0;
    virtual void finalize() = 0;
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
