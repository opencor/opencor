/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Editing view plugin
//==============================================================================

#pragma once

//==============================================================================

#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class EditingViewInterface;

//==============================================================================

namespace EditorWidget {
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace EditingView {

//==============================================================================

PLUGININFO_FUNC EditingViewPluginInfo();

//==============================================================================

class EditingViewPlugin : public QObject, public FileHandlingInterface,
                          public GuiInterface, public I18nInterface,
                          public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.EditingViewPlugin" FILE "editingviewplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit EditingViewPlugin();

#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    EditingViewInterface *mEditingViewInterface;

    QMenu *mEditMenu;

    QAction *mEditUndoAction;
    QAction *mEditRedoAction;

    QAction *mEditCutAction;
    QAction *mEditCopyAction;
    QAction *mEditPasteAction;
    QAction *mEditDeleteAction;

    QAction *mEditFindReplaceAction;
    QAction *mEditFindNextAction;
    QAction *mEditFindPreviousAction;

    QAction *mEditSelectAllAction;

    EditorWidget::EditorWidget *mEditor;

    QString mFileName;

    void updateGui(const QString &pFileName);

    void unpopulateEditMenu();
    void populateEditMenu(QAction *pBeforeAction = 0);

private slots:
    void clipboardDataChanged();

    void updateUndoAndRedoActions();
    void updateEditingActions();
    void updateFindPreviousNextActions();
    void updateSelectAllAction();

    void doUndo();
    void doRedo();

    void doCut();
    void doCopy();
    void doPaste();
    void doDelete();

    void doFindReplace();
    void doFindNext();
    void doFindPrevious();

    void doSelectAll();
};

//==============================================================================

}   // namespace EditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
