/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Plugin information
//==============================================================================

#pragma once

//==============================================================================

#include <QtPlugin>

//==============================================================================

#include <QMap>
#include <QString>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int pluginInfoVersion();

//==============================================================================

#define PLUGININFO_FUNC extern "C" Q_DECL_EXPORT void *
// Note: each plugin has a function called <PLUGIN_NAME>PluginInfo which is used
//       to retrieve some information about the plugin itself. This is done by
//       returning a PluginInfo object. This therefore requires C++ support, so
//       logically we would use extern "C++", but this would result in the
//       function name being mangled. So, to avoid this problem, we use extern
//       "C" which ensures that the function name remains intact. Now, because
//       it's C and not C++, MSVC will generate a warning if we return a
//       PluginInfo object, so we return a void pointer, which means that we
//       must free it once we have used it...

//==============================================================================

typedef QMap<QString, QString> Descriptions;

//==============================================================================

class PluginInfo
{
public:
    enum Category {
#ifdef ENABLE_SAMPLE_PLUGINS
        Sample = -3,
#endif
#ifdef ENABLE_TEST_PLUGINS
        Test = -2,
#endif
        Invalid = -1,
        Analysis,
        Api,
        DataStore,
        Editing,
        Miscellaneous,
        Organisation,
        Simulation,
        Solver,
        Support,
        ThirdParty,
        Tools,
        Widget
    };

    explicit PluginInfo(const Category &pCategory, const bool &pSelectable,
                        const bool &pCliSupport,
                        const QStringList &pDependencies,
                        const Descriptions &pDescriptions,
                        const QStringList &pLoadBefore = QStringList());

    Category category() const;

    bool isSelectable() const;
    bool hasCliSupport() const;

    QStringList dependencies() const;
    QStringList fullDependencies() const;
    void setFullDependencies(const QStringList &pFullDependencies);

    QString description(const QString &pLocale = "en") const;
    Descriptions descriptions() const;

    QStringList loadBefore() const;

private:
    Category mCategory;

    bool mSelectable;
    bool mCliSupport;

    QStringList mDependencies;
    QStringList mFullDependencies;

    Descriptions mDescriptions;

    QStringList mLoadBefore;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
