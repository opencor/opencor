//==============================================================================
// Core interface
//==============================================================================

#include "coreinterface.h"
#include "dockwidget.h"
#include "widget.h"

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

void CoreInterface::initializationsDone(const Plugins &)
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::loadSettings(QSettings *)
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::saveSettings(QSettings *) const
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::loadingOfSettingsDone(const Plugins &)
{
    // Nothing to do by default...
}

//==============================================================================

void CoreInterface::loadWindowSettings(QSettings *pSettings,
                                       Core::DockWidget *pWindow)
{
    // Retrieve the window's settings

    pSettings->beginGroup(pWindow->objectName());
        pWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CoreInterface::saveWindowSettings(QSettings *pSettings,
                                       Core::DockWidget *pWindow) const
{
    // Keep track of the window's settings

    pSettings->beginGroup(pWindow->objectName());
        pWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CoreInterface::loadViewSettings(QSettings *pSettings, QObject *pView)
{
    // Retrieve the view's settings

    pSettings->beginGroup(qobject_cast<QWidget *>(pView)->objectName());
        dynamic_cast<Core::CommonWidget *>(pView)->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CoreInterface::saveViewSettings(QSettings *pSettings, QObject *pView) const
{
    // Keep track of the view's settings

    pSettings->beginGroup(qobject_cast<QWidget *>(pView)->objectName());
        dynamic_cast<Core::CommonWidget *>(pView)->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
