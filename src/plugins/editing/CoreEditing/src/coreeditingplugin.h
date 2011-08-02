#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace CoreEditing {

PLUGININFO_FUNC CoreEditingPluginInfo();

class CoreEditingPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CoreEditingPlugin();

    virtual void initialize(QMainWindow *pMainWindow);

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

    QAction *mEditUndo;
    QAction *mEditRedo;
    QAction *mEditCut;
    QAction *mEditCopy;
    QAction *mEditPaste;
    QAction *mEditDelete;
    QAction *mEditFind;
    QAction *mEditFindNext;
    QAction *mEditPrevious;
    QAction *mEditReplace;
    QAction *mEditSelectAll;
};

} }

#endif
