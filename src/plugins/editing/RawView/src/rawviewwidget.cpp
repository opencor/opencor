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
// Raw view widget
//==============================================================================

#include "corecliutils.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawviewwidget.h"
#include "settings.h"

//==============================================================================

#include "ui_rawviewwidget.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace RawView {

//==============================================================================

RawViewWidget::RawViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::RawViewWidget),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditor(0),
    mEditors(QMap<QString, Editor::EditorWidget *>())
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

RawViewWidget::~RawViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void RawViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void RawViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void RawViewWidget::retranslateUi()
{
    // Retranslate our viewer and editor

    if (mEditor)
        mEditor->retranslateUi();
}

//==============================================================================

bool RawViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mEditors.contains(pFileName);
}

//==============================================================================

void RawViewWidget::initialize(const QString &pFileName,
                               const bool &pUpdate)
{
    // Retrieve the editor associated with the given file, if any

    Editor::EditorWidget *newEditor = mEditors.value(pFileName);

    if (!newEditor) {
        // No editor exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        newEditor = new Editor::EditorWidget(fileContents,
                                             !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                             0, parentWidget());

        // Keep track of our editor and add it to ourselves

        mEditors.insert(pFileName, newEditor);

        layout()->addWidget(newEditor);
    }

    // Update our editor, if required

    if (pUpdate) {
        Editor::EditorWidget *oldEditor = mEditor;

        mEditor = newEditor;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings(SettingsOrganization, SettingsApplication);

            settings.beginGroup(mSettingsGroup);
                newEditor->loadSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = false;
        } else {
            newEditor->updateSettings(oldEditor);
        }

        // Show/hide our editors

        newEditor->show();

        if (oldEditor && (newEditor != oldEditor))
            oldEditor->hide();

        // Set our focus proxy to our 'new' editor and make sure that the latter
        // immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editor, then the central widget would give the focus to our
        //       'old' editor (see CentralWidget::updateGui()), which is clearly
        //       not what we want...

        setFocusProxy(newEditor);

        newEditor->setFocus();
    } else {
        // Hide our 'new' editor

        newEditor->hide();
    }
}

//==============================================================================

void RawViewWidget::finalize(const QString &pFileName)
{
    // Remove the editor, should there be one for the given file

    Editor::EditorWidget *editor  = mEditors.value(pFileName);

    if (editor) {
        // There is an editor for the given file name, so save our settings and
        // reset our memory of the current editor, if needed

        if (mEditor == editor) {
            QSettings settings(SettingsOrganization, SettingsApplication);

            settings.beginGroup(mSettingsGroup);
                editor->saveSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = true;
            mEditor = 0;
        }

        // Delete the editor and remove it from our list

        delete editor;

        mEditors.remove(pFileName);
    }
}

//==============================================================================

void RawViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() would activate the contents of the
    //       view (but the file tab would still point to the previously active
    //       file). However, we want to the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    if (contains(pFileName)) {
        bool update = mEditor == mEditors.value(pFileName);

        finalize(pFileName);
        initialize(pFileName, update);
    }
}

//==============================================================================

void RawViewWidget::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // The given file has been renamed, so update our editors mapping

    Editor::EditorWidget *editor = mEditors.value(pOldFileName);

    if (editor) {
        mEditors.insert(pNewFileName, editor);
        mEditors.remove(pOldFileName);
    }
}

//==============================================================================

Editor::EditorWidget * RawViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    return mEditors.value(pFileName);
}

//==============================================================================

QList<QWidget *> RawViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditor)
        return QList<QWidget *>() << mEditor->cursorPositionWidget()
                                  << mEditor->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
