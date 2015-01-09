/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CoreEditing plugin
//==============================================================================

#ifndef COREEDITINGPLUGIN_H
#define COREEDITINGPLUGIN_H

//==============================================================================

#include "filehandlinginterface.h"
#include "guiinterface.h"
#include "i18ninterface.h"
#include "plugininfo.h"
#include "plugininterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class EditingInterface;

//==============================================================================

namespace Editor {
    class EditorWidget;
}   // namespace Editor

//==============================================================================

namespace CoreEditing {

//==============================================================================

PLUGININFO_FUNC CoreEditingPluginInfo();

//==============================================================================

class CoreEditingPlugin : public QObject, public FileHandlingInterface,
                          public GuiInterface, public I18nInterface,
                          public PluginInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CoreEditingPlugin" FILE "coreeditingplugin.json")

    Q_INTERFACES(OpenCOR::FileHandlingInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::PluginInterface)

public:
    explicit CoreEditingPlugin();

#include "filehandlinginterface.inl"
#include "guiinterface.inl"
#include "i18ninterface.inl"
#include "plugininterface.inl"

private:
    EditingInterface *mEditingInterface;

    QAction *mFileNewFileAction;

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

    Editor::EditorWidget *mEditor;

    QString mFileName;

    void updateGui(const QString &pFileName);

    void unpopulateEditMenu();
    void populateEditMenu(QAction *pBeforeAction = 0);

private Q_SLOTS:
    void clipboardDataChanged();

    void updateUndoAndRedoActions();
    void updateEditingActions();
    void updateFindPreviousNextActions();
    void updateSelectAllAction();

    void newFile();

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

}   // namespace CoreEditing
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
