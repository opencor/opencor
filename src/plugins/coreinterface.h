#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include <QtPlugin>

#include <QMap>
#include <QStringList>

namespace OpenCOR {

static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString FrenchLocale  = "fr";

class Plugin;

class CoreInterface : public QObject
{
public:
    virtual void initialize();
    virtual void finalize();

    void setParameters(const QList<Plugin *> &pLoadedPlugins);

protected:
    QList<Plugin *> mLoadedPlugins;
};

#define PLUGININFO_FUNC extern "C" Q_DECL_EXPORT PluginInfo

typedef QMap<QString, QString> Descriptions;

class PluginInfo
{
    friend class Plugin;

public:
    enum Version
    {
        UndefinedVersion,
        V001
    };

    enum Type
    {
        UndefinedType,
        General,
        Console,
        Gui
    };

    enum Category
    {
        Application,
        Api,
        Organisation,
        Editing,
        Simulation,
        Analysis,
        ThirdParty
    };

    explicit PluginInfo(const Version &pVersion = UndefinedVersion,
                        const Type &pType = UndefinedType,
                        const Category &pCategory = Application,
                        const bool &pManageable = false,
                        const QStringList &pDependencies = QStringList(),
                        const Descriptions &pDescriptions = Descriptions());

    Version version() const;
    Type type() const;
    Category category() const;
    bool manageable() const;
    QStringList dependencies() const;
    QStringList fullDependencies() const;
    QString description(const QString &pLocale = EnglishLocale) const;

private:
    Version mVersion;
    Type mType;
    Category mCategory;
    bool mManageable;
    QStringList mDependencies;
    QStringList mFullDependencies;
    Descriptions mDescriptions;
};

QString description(const Descriptions &pDescriptions, const QString &pLocale);

}

Q_DECLARE_INTERFACE(OpenCOR::CoreInterface, "OpenCOR.CoreInterface")

#endif
