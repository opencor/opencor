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

class CoreInterface : public Interface
{
public:
    virtual void initialize();
    virtual void finalize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &pLoadedPlugins);

    virtual void handleArguments(const QStringList &pArguments);
    virtual void handleAction(const QUrl &pUrl);
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::CoreInterface, "OpenCOR::CoreInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
