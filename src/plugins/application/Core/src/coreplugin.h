#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "guiinterface.h"
#include "apiinterface.h"

#include <QDir>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo();

class CentralWidget;

class CorePlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CorePlugin();

    virtual void initialize(const QList<Plugin *> &pPlugins,
                            QMainWindow *pMainWindow);
    virtual void finalize();

    virtual void retranslateUi();

private:
    QList<FileType> mSupportedFileTypes;

    CentralWidget *mCentralWidget;

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
