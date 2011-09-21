#include "plugininterface.h"

namespace OpenCOR {

FileType::FileType(const QString &pMimeType, const QString &pFileExtension,
                   const FileDescriptions &pDescriptions) :
    mMimeType(pMimeType),
    mFileExtension(pFileExtension),
    mDescriptions(pDescriptions)
{
}

QString FileType::mimeType() const
{
    // Return the file's Mime type

    return mMimeType;
}

QString FileType::fileExtension() const
{
    // Return the file's extension

    return mFileExtension;
}

FileDescriptions FileType::descriptions() const
{
    // Return the file's descriptions

    return mDescriptions;
}

void PluginInterface::initialize(const QList<Plugin *> &)
{
    // Nothing to do by default...
}

void PluginInterface::finalize()
{
    // Nothing to do by default...
}

QList<FileType> PluginInterface::fileTypes()
{
    // By default, there are no file types

    return QList<FileType>();
}

PluginInfo::PluginInfo(const PluginInterface::Version &pInterfaceVersion,
                       const Type &pType, const Category &pCategory,
                       const bool &pManageable,
                       const QStringList &pDependencies,
                       const PluginInfoDescriptions &pDescriptions) :
    mInterfaceVersion(pInterfaceVersion),
    mType(pType),
    mCategory(pCategory),
    mManageable(pManageable),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions)
{
}

PluginInterface::Version PluginInfo::interfaceVersion() const
{
    // Return the interface version used by the plugin

    return mInterfaceVersion;
}

PluginInfo::Type PluginInfo::type() const
{
    // Return the plugin's type

    return mType;
}

PluginInfo::Category PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

bool PluginInfo::manageable() const
{
    // Return the plugin's manageability

    return mManageable;
}

QStringList PluginInfo::dependencies() const
{
    // Return the plugin's dependencies

    return mDependencies;
}

QStringList PluginInfo::fullDependencies() const
{
    // Return the plugin's full dependencies
    // Note: they are determined by the plugin itself (i.e. during the
    //       construction of a Plugin object)

    return mFullDependencies;
}

QString PluginInfo::description(const QString &pLocale) const
{
    // Return the plugin's description using the provided locale or the first
    // description if no description can be found for the provided locale

    if (mDescriptions.isEmpty()) {
        // No description is avalable, so...

        return QString();
    } else {
        // At least one description is available, so return the one that
        // matches our locale our the first description if there is no match

        QString res = mDescriptions.value(pLocale);

        return res.isEmpty()?mDescriptions.begin().value():res;
    }
}

}
