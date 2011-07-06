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

class PluginInfo
{
    friend class Plugin;

public:
    enum PluginType
    {
        Undefined,
        General,
        Console,
        Gui
    };

    explicit PluginInfo(const PluginType &pType = Undefined,
                        const QStringList &pDependencies = QStringList());

    PluginType type();
    QStringList dependencies();

private:
    PluginType mType;
    QStringList mDependencies;
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
