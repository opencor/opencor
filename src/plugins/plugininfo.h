/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

static const auto AnalysisCategory      = QStringLiteral("Analysis");
static const auto ApiCategory           = QStringLiteral("API");
static const auto DataStoreCategory     = QStringLiteral("Data Store");
static const auto EditingCategory       = QStringLiteral("Editing");
static const auto MiscellaneousCategory = QStringLiteral("Miscellaneous");
static const auto OrganisationCategory  = QStringLiteral("Organisation");
#ifdef ENABLE_SAMPLES
static const auto SampleCategory        = QStringLiteral("Sample");
#endif
static const auto SimulationCategory    = QStringLiteral("Simulation");
static const auto SolverCategory        = QStringLiteral("Solver");
static const auto SupportCategory       = QStringLiteral("Support");
static const auto ThirdPartyCategory    = QStringLiteral("Third-party");
static const auto ToolsCategory         = QStringLiteral("Tools");
static const auto WidgetCategory        = QStringLiteral("Widget");

//==============================================================================

typedef QMap<QString, QString> Descriptions;

//==============================================================================

class PluginInfo
{
public:
    explicit PluginInfo(const QString &pCategory, const bool &pSelectable,
                        const bool &pCliSupport,
                        const QStringList &pDependencies,
                        const Descriptions &pDescriptions,
                        const QStringList &pLoadBefore = QStringList());

    QString category() const;

    bool isSelectable() const;
    bool hasCliSupport() const;

    QStringList dependencies() const;
    QStringList fullDependencies() const;
    void setFullDependencies(const QStringList &pFullDependencies);

    QString description(const QString &pLocale = "en") const;
    Descriptions descriptions() const;

    QStringList loadBefore() const;

private:
    QString mCategory;

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
