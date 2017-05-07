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
// CLI application
//==============================================================================

#pragma once

//==============================================================================

#include "plugin.h"

//==============================================================================

#include <QStringList>

//==============================================================================

class QCoreApplication;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginManager;

//==============================================================================

class CliApplication {
public:
    explicit CliApplication(int &pArgC, char **pArgV);
    ~CliApplication();

    bool run(int *pRes);

private:
    QCoreApplication *mCliApplication;

    PluginManager *mPluginManager;

    Plugins mLoadedPluginPlugins;
    Plugins mLoadedCliPlugins;

    void loadPlugins();
    void includePlugins(const QStringList &pPluginNames,
                        const bool &pInclude = true) const;

    void about() const;
    bool command(const QStringList &pArguments, int *pRes) const;
    void exclude(const QStringList &pPluginNames) const;
    void help() const;
    void include(const QStringList &pPluginNames) const;
    void plugins() const;
    void reset() const;
    void status() const;
    void version() const;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
