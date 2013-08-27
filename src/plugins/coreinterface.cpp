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
// Core interface
//==============================================================================

#include "coreinterface.h"

//==============================================================================

#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void CoreInterface::initialize()
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::finalize()
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::initializationsDone(const Plugins &pLoadedPlugins)
{
    // Nothing to do by default...

    Q_UNUSED(pLoadedPlugins);
}

//==============================================================================

void CoreInterface::loadSettings(QSettings *pSettings)
{
    // Nothing to do by default...

    Q_UNUSED(pSettings);
}

//==============================================================================

void CoreInterface::saveSettings(QSettings *pSettings) const
{
    // Nothing to do by default...

    Q_UNUSED(pSettings);
}

//==============================================================================

void CoreInterface::loadingOfSettingsDone(const Plugins &pLoadedPlugins)
{
    // Nothing to do by default...

    Q_UNUSED(pLoadedPlugins)
}

//==============================================================================

void CoreInterface::handleArguments(const QStringList &pArguments)
{
    // Nothing to do by default...

    Q_UNUSED(pArguments);
}

//==============================================================================

void CoreInterface::handleAction(const QUrl &pUrl)
{
    // Nothing to do by default...

    Q_UNUSED(pUrl);
}

//==============================================================================

void CoreInterface::runCliCommand(const QString &pCommand,
                                  const QStringList &pArguments, int *pRes)
{
    // Nothing to do by default...

    Q_UNUSED(pCommand);
    Q_UNUSED(pArguments);

    *pRes = 0;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
