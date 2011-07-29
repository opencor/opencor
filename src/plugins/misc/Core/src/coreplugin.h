#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "guiinterface.h"

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

    virtual void initialize(QMainWindow *pMainWindow);
    virtual void finalize();

    virtual void retranslateUi();

private:
    QAction *mFileOpen;
    QAction *mFileSave;
    QAction *mFileSaveAs;
    QAction *mFileSaveAll;
    QAction *mFileClose;
    QAction *mFileCloseAll;
    QAction *mFilePrint;

    QMenu *mEdit;

    CentralWidget *mCentralWidget;
};

} }

#endif
