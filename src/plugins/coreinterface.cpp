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

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
