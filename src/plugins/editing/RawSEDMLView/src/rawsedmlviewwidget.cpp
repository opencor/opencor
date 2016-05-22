/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Raw SED-ML view widget
//==============================================================================

#include "corecliutils.h"
#include "coresedmleditingwidget.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawsedmlviewwidget.h"
#include "sedmlfile.h"
#include "sedmlfilemanager.h"
#include "sedmlfileissue.h"

//==============================================================================

#include <QDomDocument>
#include <QLabel>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QVariant>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawSEDMLView {

//==============================================================================

RawSedmlViewWidget::RawSedmlViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreSEDMLEditing::CoreSedmlEditingWidget *>())
{
}

//==============================================================================

void RawSedmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void RawSedmlViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void RawSedmlViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    foreach (CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
}

//==============================================================================

bool RawSedmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mEditingWidgets.contains(pFileName);
}

//==============================================================================

void RawSedmlViewWidget::initialize(const QString &pFileName,
                                    const bool &pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CoreSEDMLEditing::CoreSedmlEditingWidget *newEditingWidget = mEditingWidgets.value(pFileName);

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so create one

        QByteArray fileContents;

        Core::readFileContentsFromFile(pFileName, fileContents);

        newEditingWidget = new CoreSEDMLEditing::CoreSedmlEditingWidget(fileContents,
                                                                        !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                        new QsciLexerXML(this),
                                                                        parentWidget());

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, newEditingWidget);

        layout()->addWidget(newEditingWidget);
    }

    // Update our editing widget, if required

    if (pUpdate) {
        CoreSEDMLEditing::CoreSedmlEditingWidget *oldEditingWidget = mEditingWidget;

        mEditingWidget = newEditingWidget;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                newEditingWidget->loadSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = false;
        } else {
            newEditingWidget->updateSettings(oldEditingWidget);
        }

        // Show/hide our editing widgets

        newEditingWidget->show();

        if (oldEditingWidget && (newEditingWidget != oldEditingWidget))
            oldEditingWidget->hide();

        // Set our focus proxy to our 'new' editing widget and make sure that
        // the latter immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editing widget, then the central widget would give the focus to
        //       our 'old' editing widget (see CentralWidget::updateGui()),
        //       which is clearly not what we want...

        setFocusProxy(newEditingWidget->editor());

        newEditingWidget->editor()->setFocus();
    } else {
        // Hide our 'new' editing widget

        newEditingWidget->hide();
    }
}

//==============================================================================

void RawSedmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        if (mEditingWidget == editingWidget) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                editingWidget->saveSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = true;
            mEditingWidget = 0;
        }

        // Delete the editor and remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);
    }
}

//==============================================================================

void RawSedmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() would activate the contents of the
    //       view (but the file tab would still point to the previously active
    //       file). However, we want to the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    if (contains(pFileName)) {
        bool update = mEditingWidget == mEditingWidgets.value(pFileName);

        finalize(pFileName);
        initialize(pFileName, update);
    }
}

//==============================================================================

void RawSedmlViewWidget::fileRenamed(const QString &pOldFileName,
                                     const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

Editor::EditorWidget * RawSedmlViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?editingWidget->editor():0;
}

//==============================================================================

QList<QWidget *> RawSedmlViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget)
        return QList<QWidget *>() << mEditingWidget->editor()->cursorPositionWidget()
                                  << mEditingWidget->editor()->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

void RawSedmlViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget && validate(pFileName, true)) {
        int cursorLine;
        int cursorColumn;

        editingWidget->editor()->cursorPosition(cursorLine, cursorColumn);

        QDomDocument domDocument;

        domDocument.setContent(editingWidget->editor()->contents());

        editingWidget->editor()->setContents(Core::serialiseDomDocument(domDocument), true);
        editingWidget->editor()->setCursorPosition(cursorLine, cursorColumn);
    }
}

//==============================================================================

bool RawSedmlViewWidget::validate(const QString &pFileName,
                                  const bool &pOnlyErrors) const
{
    // Validate the given file

    CoreSEDMLEditing::CoreSedmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // Clear the list of SED-ML issues

        EditorList::EditorListWidget *editorList = editingWidget->editorList();

        editorList->clear();

        // Retrieve the list of SED-ML issues, if any

        SEDMLSupport::SedmlFile *sedmlFile = SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName);
        SEDMLSupport::SedmlFileIssues sedmlFileIssues;

        bool res = sedmlFile->isValid(editingWidget->editor()->contents(), sedmlFileIssues);

        // Add whatever issue there may be to our list and select the first one
        // of them

        foreach (const SEDMLSupport::SedmlFileIssue &sedmlFileIssue, sedmlFileIssues) {
            if (   !pOnlyErrors
                || (sedmlFileIssue.type() == SEDMLSupport::SedmlFileIssue::Error)) {
                EditorList::EditorListItem::Type issueType;

                switch (sedmlFileIssue.type()) {
                case SEDMLSupport::SedmlFileIssue::Information:
                    issueType = EditorList::EditorListItem::Information;

                    break;
                    case SEDMLSupport::SedmlFileIssue::Error:
                    issueType = EditorList::EditorListItem::Error;

                    break;
                    case SEDMLSupport::SedmlFileIssue::Warning:
                    issueType = EditorList::EditorListItem::Warning;

                    break;
                    case SEDMLSupport::SedmlFileIssue::Fatal:
                    issueType = EditorList::EditorListItem::Fatal;

                    break;
                }

                editorList->addItem(issueType, sedmlFileIssue.line(), sedmlFileIssue.column(),
                                    qPrintable(sedmlFileIssue.message()));
            }
        }

        editorList->selectFirstItem();

        return res;
    } else {
        // The file doesn't exist, so it can't be validated

        return false;
    }
}

//==============================================================================

}   // namespace RawSEDMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
