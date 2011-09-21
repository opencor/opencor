#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include <QMap>
#include <QStringList>

namespace OpenCOR {

class Plugin;

typedef QMap<QString, QString> FileDescriptions;

class FileType
{
public:
    explicit FileType(const QString &pMimeType, const QString &pFileExtension,
                      const FileDescriptions &pDescriptions);

    QString mimeType() const;
    QString fileExtension() const;
    FileDescriptions descriptions() const;

private:
    QString mMimeType;
    QString mFileExtension;
    FileDescriptions mDescriptions;
};

class PluginInterface : public QObject
{
public:
    enum Version
    {
        Undefined,
        V001
    };

    virtual void initialize(const QList<Plugin *> &);
    virtual void finalize();

    virtual QList<FileType> fileTypes();
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
    enum Type
    {
        Undefined,
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

    explicit PluginInfo(const PluginInterface::Version &pInterfaceVersion = PluginInterface::Undefined,
                        const Type &pType = Undefined,
                        const Category &pCategory = Application,
                        const bool &pManageable = false,
                        const QStringList &pDependencies = QStringList(),
                        const PluginInfoDescriptions &pDescriptions = PluginInfoDescriptions());

    PluginInterface::Version interfaceVersion() const;
    Type type() const;
    Category category() const;
    bool manageable() const;
    QStringList dependencies() const;
    QStringList fullDependencies() const;
    QString description(const QString &pLocale = "en") const;

private:
    PluginInterface::Version mInterfaceVersion;
    Type mType;
    Category mCategory;
    bool mManageable;
    QStringList mDependencies;
    QStringList mFullDependencies;
    PluginInfoDescriptions mDescriptions;
};

}

Q_DECLARE_INTERFACE(OpenCOR::PluginInterface, "OpenCOR.PluginInterface")

#endif
