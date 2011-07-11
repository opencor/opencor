#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include <QMap>
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
                        const QStringList &pDependencies = QStringList(),
                        const QMap<QString, QString> &pDescriptions = QMap<QString, QString>());

    PluginType type();
    QStringList dependencies();
    QString description(const QString &pLocale = "en");

private:
    PluginType mType;
    QStringList mDependencies;
    QMap<QString, QString> mDescriptions;
};

class PluginInterface : public QObject
{
public:
    virtual void initialize();
    virtual void finalize();
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
