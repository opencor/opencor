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
