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
// CLI application
//==============================================================================

#ifndef CLIAPPLICATION_H
#define CLIAPPLICATION_H

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
    explicit CliApplication(QCoreApplication *pApp);
    ~CliApplication();

    bool run(int *pRes);

private:
    QCoreApplication *mApp;

    PluginManager *mPluginManager;

    Plugins mLoadedCliPlugins;

    void loadPlugins();

    void about();
    bool command(const QStringList pArguments, int *pRes);
    void help();
    void plugins();
    void status();
    void version();
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
