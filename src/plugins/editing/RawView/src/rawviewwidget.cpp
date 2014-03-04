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

#include "cliutils.h"
#include "filemanager.h"
#include "qscintillawidget.h"
#include "rawviewwidget.h"

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
    mEditor(0),
    mEditors(QMap<QString, QScintillaSupport::QScintillaWidget *>()),
    mEditorZoomLevel(0)
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

static const auto SettingsEditorZoomLevel = QStringLiteral("EditorZoomLevel");

//==============================================================================

void RawViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the editor's zoom level

    mEditorZoomLevel = pSettings->value(SettingsEditorZoomLevel, 0).toInt();
}

//==============================================================================

void RawViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the editor's zoom level

    pSettings->setValue(SettingsEditorZoomLevel, mEditorZoomLevel);
}

//==============================================================================

bool RawViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file, i.e. whether we have an
    // editor for it

    return mEditors.value(pFileName);
}

//==============================================================================

void RawViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editor associated with the given file, if any

    mEditor = mEditors.value(pFileName);

    if (!mEditor) {
        // No editor exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        mEditor = new QScintillaSupport::QScintillaWidget(fileContents,
                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                          0, parentWidget());

        // Keep track of changes to our editor's zoom level

        connect(mEditor, SIGNAL(SCN_ZOOM()),
                this, SLOT(editorZoomLevelChanged()));

        // Keep track of our editor and add it to ourselves

        mEditors.insert(pFileName, mEditor);

        layout()->addWidget(mEditor);
    }

    // Show/hide our editors and adjust our sizes

    foreach (QScintillaSupport::QScintillaWidget *editor, mEditors)
        if (editor == mEditor) {
            // This is the editor we are after, so show it and update its zoom
            // level

            editor->zoomTo(mEditorZoomLevel);

            editor->show();
        } else {
            // Not the editor we are after, so hide it

            editor->hide();
        }

    // Set our focus proxy to our 'new' editor and make sure that the latter
    // immediately gets the focus
    // Note: if we were not to immediately give our 'new' editor the focus, then
    //       the central widget would give the focus to our 'old' editor (see
    //       CentralWidget::updateGui()), so...

    setFocusProxy(mEditor);

    mEditor->setFocus();
}

//==============================================================================

void RawViewWidget::finalize(const QString &pFileName)
{
    // Remove the editor, should there be one for the given file

    QScintillaSupport::QScintillaWidget *editor  = mEditors.value(pFileName);

    if (editor) {
        // There is an editor for the given file name, so delete it and remove
        // it from our list

        delete editor;

        mEditors.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editor == mEditor)
            mEditor = 0;
    }
}

//==============================================================================

void RawViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void RawViewWidget::fileRenamed(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // The given file has been renamed, so update our editors mapping

    QScintillaSupport::QScintillaWidget *editor = mEditors.value(pOldFileName);

    if (editor) {
        mEditors.insert(pNewFileName, editor);
        mEditors.remove(pOldFileName);
    }
}

//==============================================================================

QScintillaSupport::QScintillaWidget * RawViewWidget::editor(const QString &pFileName) const
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

void RawViewWidget::editorZoomLevelChanged()
{
    // One of our editors had its zoom level changed, so keep track of the new
    // zoom level

    mEditorZoomLevel = qobject_cast<QScintillaSupport::QScintillaWidget *>(sender())->SendScintilla(QsciScintillaBase::SCI_GETZOOM);
}

//==============================================================================

}   // namespace RawView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
