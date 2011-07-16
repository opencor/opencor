#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include <QMap>
#include <QStringList>

namespace OpenCOR {

class PluginInterface : public QObject
{
public:
    enum PluginInterfaceVersion
    {
        Undefined,
        V001
    };

    virtual void initialize();
    virtual void finalize();
};

#ifdef Q_WS_WIN
    #define PLUGININFO_FUNC extern "C" __declspec(dllexport) PluginInfo
#else
    #define PLUGININFO_FUNC extern "C" PluginInfo
#endif

typedef QMap<QString, QString> PluginInfoDescriptions;

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

    explicit PluginInfo(const PluginInterface::PluginInterfaceVersion &pPluginInterfaceVersion = PluginInterface::Undefined,
                        const PluginType &pType = Undefined,
                        const bool &pManageable = false,
                        const QStringList &pDependencies = QStringList(),
                        const PluginInfoDescriptions &pDescriptions = PluginInfoDescriptions());

    PluginInterface::PluginInterfaceVersion pluginInterfaceVersion() const;
    PluginType type() const;
    bool manageable() const;
    QStringList dependencies() const;
    QStringList fullDependencies() const;
    QString description(const QString &pLocale = "en") const;

private:
    PluginInterface::PluginInterfaceVersion mPluginInterfaceVersion;
    PluginType mType;
    bool mManageable;
    QStringList mDependencies;
    QStringList mFullDependencies;
    PluginInfoDescriptions mDescriptions;
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
