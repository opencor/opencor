//==============================================================================
// Viewer plugin
//==============================================================================

#ifndef VIEWERPLUGIN_H
#define VIEWERPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

PLUGININFO_FUNC ViewerPluginInfo();

//==============================================================================

class ViewerWindow;

//==============================================================================

class ViewerPlugin : public QObject, public CoreInterface, public GuiInterface,
                     public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

protected:
    virtual void retranslateUi();

private:
    QAction *mViewerAction;

    ViewerWindow *mViewerWindow;
};

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
