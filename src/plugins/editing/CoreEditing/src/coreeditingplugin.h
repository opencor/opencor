#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

#include "guiinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CoreEditing {

PLUGININFO_FUNC CoreEditingPluginInfo();

static const QString EditGroup = "Edit";

class CoreEditingPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CoreEditingPlugin();

    virtual void initialize();

protected:
    virtual void retranslateUi();

private:
    QMenu *mEditMenu;

    QToolBar *mEditToolbar;
    QAction *mEditToolbarAction;

    QAction *mEditUndoAction;
    QAction *mEditRedoAction;
    QAction *mEditCutAction;
    QAction *mEditCopyAction;
    QAction *mEditPasteAction;
    QAction *mEditDeleteAction;
    QAction *mEditFindAction;
    QAction *mEditFindNextAction;
    QAction *mEditPreviousAction;
    QAction *mEditReplaceAction;
    QAction *mEditSelectAllAction;
};

} }

#endif
