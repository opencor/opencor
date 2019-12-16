/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
} // namespace EditorWidget

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
#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    EditingViewInterface *mEditingViewInterface = nullptr;

    QMenu *mEditMenu = nullptr;

    QAction *mEditUndoAction = nullptr;
    QAction *mEditRedoAction = nullptr;

    QAction *mEditCutAction = nullptr;
    QAction *mEditCopyAction = nullptr;
    QAction *mEditPasteAction = nullptr;
    QAction *mEditDeleteAction = nullptr;

    QAction *mEditFindReplaceAction = nullptr;
    QAction *mEditFindNextAction = nullptr;
    QAction *mEditFindPreviousAction = nullptr;

    QAction *mEditSelectAllAction = nullptr;

    QAction *mEditWordWrapAction = nullptr;

    EditorWidget::EditorWidget *mEditor = nullptr;

    QString mFileName;

    void updateGui(const QString &pFileName);

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

    void doWordWrap();
};

//==============================================================================

} // namespace EditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
