#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace CoreEditing {

PLUGININFO_FUNC CoreEditingPluginInfo();

static const QString EditMenu = "Edit";

class CoreEditingPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CoreEditingPlugin();

    virtual void initialize();

protected:
    virtual void retranslateUi();

private:
    QMenu *mEditMenu;
    QToolBar *mEditToolbar;

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
