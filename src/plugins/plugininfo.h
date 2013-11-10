/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

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

#define PLUGININFO_FUNC extern "C" Q_DECL_EXPORT void *
// Note: each plugin has a function called <PLUGIN_NAME>PluginInfo which can be
//       used to retrieve some information about the plugin itself. This is done
//       by returning a PluginInfo object. This therefore requires C++ support,
//       so logically we would use extern "C++", but this would result in the
//       function name being mangled. So, to avoid this problem, we use extern
//       "C" which ensures that the function name remains intact. Now, because
//       it's C and not C++, MSVC will generate a warning if we return a
//       PluginInfo object, so we return a void pointer which means that we must
//       free it once we have used...

//==============================================================================

typedef QMap<QString, QString> Descriptions;

//==============================================================================

class PluginInfo
{
public:
    enum Category {
        Organisation,
        Editing,
        Simulation,
        Analysis,
        Miscellaneous,
        Api,
        ThirdParty
    };

    explicit PluginInfo(const Category &pCategory, const bool &pCliSupport,
                        const bool &pManageable,
                        const QStringList &pDependencies,
                        const Descriptions &pDescriptions);

    Category category() const;

    bool hasCliSupport() const;
    bool isManageable() const;

    QStringList dependencies() const;
    QStringList fullDependencies() const;
    void setFullDependencies(const QStringList &pFullDependencies);

    QString description(const QString &pLocale = "en") const;
    Descriptions descriptions() const;

private:
    Category mCategory;

    bool mCliSupport;
    bool mManageable;

    QStringList mDependencies;
    QStringList mFullDependencies;

    Descriptions mDescriptions;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
