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

class QSettings;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {

//==============================================================================

class DockWidget;
class Widget;

//==============================================================================

}

//==============================================================================

class CoreInterface : Interface
{
    friend class MainWindow;

public:
    virtual void initialize();
    virtual void finalize();

    virtual void initializationsDone(const Plugins &);

    virtual void loadSettings(QSettings *);
    virtual void saveSettings(QSettings *) const;

    virtual void loadingOfSettingsDone(const Plugins &);

    void loadWindowSettings(QSettings *pSettings,
                            Core::DockWidget *pWindow);
    void saveWindowSettings(QSettings *pSettings,
                            Core::DockWidget *pWindow) const;

    void loadViewSettings(QSettings *pSettings, Core::Widget *pView);
    void saveViewSettings(QSettings *pSettings, Core::Widget *pView) const;
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
