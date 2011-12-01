//==============================================================================
// Plugin information
//==============================================================================

#ifndef PLUGININFO_H
#define PLUGININFO_H

//==============================================================================

#include <QtPlugin>

//==============================================================================

#include <QMap>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString FrenchLocale  = "fr";

//==============================================================================

#define PLUGININFO_FUNC extern "C" Q_DECL_EXPORT PluginInfo

//==============================================================================

typedef QMap<QString, QString> Descriptions;

//==============================================================================

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
        Miscellaneous,
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

//==============================================================================

QString description(const Descriptions &pDescriptions, const QString &pLocale);

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
