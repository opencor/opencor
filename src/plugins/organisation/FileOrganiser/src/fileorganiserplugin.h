//==============================================================================
// FileOrganiser plugin
//==============================================================================

#ifndef FILEORGANISERPLUGIN_H
#define FILEORGANISERPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

PLUGININFO_FUNC FileOrganiserPluginInfo();

//==============================================================================

class FileOrganiserWindow;

//==============================================================================

class FileOrganiserPlugin : public QObject, public CoreInterface,
                            public GuiInterface, public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.FileOrganiserPlugin" FILE "fileorganiserplugin.json")

    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

private:
    QAction *mFileOrganiserAction;

    FileOrganiserWindow *mFileOrganiserWindow;
};

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
