//==============================================================================
// Core plugin
//==============================================================================

#ifndef COREPLUGIN_H
#define COREPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "fileinterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QDir>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PLUGININFO_FUNC CorePluginInfo();

//==============================================================================

static const QString FileGroup = "File";

//==============================================================================

class CentralWidget;

//==============================================================================

class CorePlugin : public QObject, public CoreInterface, public GuiInterface,
                   public I18nInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)

public:
    virtual void initialize();

    virtual void initializationsDone(const Plugins &pLoadedPlugins);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void loadingOfSettingsDone(const Plugins &pPlugins);

    virtual void retranslateUi();

private:
    QList<FileType> mSupportedFileTypes;

    CentralWidget *mCentralWidget;

    QAction *mFileOpenAction;
    QAction *mFileSaveAction;
    QAction *mFileSaveAsAction;
    QAction *mFileSaveAllAction;
    QAction *mFilePreviousAction;
    QAction *mFileNextAction;
    QAction *mFileCloseAction;
    QAction *mFileCloseAllAction;
    QAction *mFilePrintAction;

    QDir mActiveDir;

private Q_SLOTS:
    void openFile();
};

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
