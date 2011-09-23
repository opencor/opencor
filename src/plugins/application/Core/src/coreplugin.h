#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "guiinterface.h"
#include "apiinterface.h"

#include <QDir>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo();

static const QString FileGroup = "file";

class CentralWidget;

class CorePlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CorePlugin();

    virtual void initialize();
    virtual void finalize();

    virtual void loadSettings(QSettings *pSettings,
                              const bool &pNeedDefaultSettings);
    virtual void saveSettings(QSettings *pSettings) const;

protected:
    virtual void retranslateUi();

private:
    QList<FileType> mSupportedFileTypes;

    CentralWidget *mCentralWidget;

    QToolBar *mFileToolbar;
    QAction *mFileToolbarAction;

    QAction *mFileOpen;
    QAction *mFileSave;
    QAction *mFileSaveAs;
    QAction *mFileSaveAll;
    QAction *mFileClose;
    QAction *mFileCloseAll;
    QAction *mFilePrint;

    QDir mActiveDir;

private Q_SLOTS:
    void openFile();
};

} }

#endif
