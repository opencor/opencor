//==============================================================================
// Core interface
//==============================================================================

#ifndef COREINTERFACE_H
#define COREINTERFACE_H

//==============================================================================

#include "interface.h"
#include "plugin.h"

//==============================================================================

#include <QtPlugin>

//==============================================================================

#include <QUrl>

//==============================================================================

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class CommonWidget;
    class DockWidget;
}

//==============================================================================

class CoreInterface : public Interface
{
public:
    virtual void initialize();
    virtual void finalize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &pLoadedPlugins);

    virtual bool handleArguments(const QStringList &pArguments);
    virtual bool handleAction(const QUrl &pUrl);

protected:
    void loadWindowSettings(QSettings *pSettings,
                            Core::DockWidget *pWindow);
    void saveWindowSettings(QSettings *pSettings,
                            Core::DockWidget *pWindow) const;

    void loadViewSettings(QSettings *pSettings, QObject *pView);
    void saveViewSettings(QSettings *pSettings, QObject *pView) const;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::CoreInterface, "OpenCOR.CoreInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
