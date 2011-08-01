#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include <QMap>
#include <QStringList>

namespace OpenCOR {

class PluginInterface : public QObject
{
public:
    enum Version
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

    enum PluginCategory
    {
        Application,
        Api,
        Organisation,
        Editing,
        Simulation,
        Analysis,
        ThirdParty
    };

    explicit PluginInfo(const PluginInterface::Version &pInterfaceVersion = PluginInterface::Undefined,
                        const PluginType &pType = Undefined,
                        const PluginCategory &pCategory = Application,
                        const bool &pManageable = false,
                        const QStringList &pDependencies = QStringList(),
                        const PluginInfoDescriptions &pDescriptions = PluginInfoDescriptions());

    PluginInterface::Version interfaceVersion() const;
    PluginType type() const;
    PluginCategory category() const;
    bool manageable() const;
    QStringList dependencies() const;
    QStringList fullDependencies() const;
    QString description(const QString &pLocale = "en") const;

private:
    PluginInterface::Version mInterfaceVersion;
    PluginType mType;
    PluginCategory mCategory;
    bool mManageable;
    QStringList mDependencies;
    QStringList mFullDependencies;
    PluginInfoDescriptions mDescriptions;
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
