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
// Raw SED-ML view widget
//==============================================================================

#include "corecliutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawsedmlviewwidget.h"
#include "sedmleditingviewwidget.h"
#include "sedmlfile.h"
#include "sedmlfileissue.h"
#include "sedmlfilemanager.h"

//==============================================================================

#include <QDomDocument>
#include <QLabel>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QVariant>

//==============================================================================

#include "qscintillabegin.h"
    #include "Qsci/qscilexerxml.h"
#include "qscintillaend.h"

//==============================================================================

namespace OpenCOR {
namespace RawSEDMLView {

//==============================================================================

RawSedmlViewWidget::RawSedmlViewWidget(QWidget *pParent) :
    ViewWidget(pParent)
{
}

//==============================================================================

void RawSedmlViewWidget::loadSettings(QSettings &pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings.group();
}

//==============================================================================

void RawSedmlViewWidget::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void RawSedmlViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    for (auto editingWidget : mEditingWidgets) {
        editingWidget->retranslateUi();
    }
}

//==============================================================================

void RawSedmlViewWidget::initialize(const QString &pFileName, bool pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    SEDMLEditingView::SedmlEditingViewWidget *newEditingWidget = mEditingWidgets.value(pFileName);

    if (newEditingWidget == nullptr) {
        // No editing widget exists for the given file, so create one

        QByteArray fileContents;

        Core::readFile(pFileName, fileContents);

        newEditingWidget = new SEDMLEditingView::SedmlEditingViewWidget(fileContents,
                                                                        !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                        new QsciLexerXML(this),
                                                                        parentWidget());

        // Keep track of our editing widget

        mEditingWidgets.insert(pFileName, newEditingWidget);
    }

    // Update our editing widget, if required

    if (pUpdate) {
        SEDMLEditingView::SedmlEditingViewWidget *oldEditingWidget = mEditingWidget;

        mEditingWidget = newEditingWidget;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);

            newEditingWidget->loadSettings(settings);

            mNeedLoadingSettings = false;
        } else {
            newEditingWidget->updateSettings(oldEditingWidget);
        }

        // Set our focus proxy to our 'new' editing widget and make sure that
        // the latter immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editing widget, then the central widget would give the focus to
        //       our 'old' editing widget (see CentralWidget::updateGui()),
        //       which is clearly not what we want...

        setFocusProxy(newEditingWidget->editorWidget());

        newEditingWidget->editorWidget()->setFocus();
    } else {
        // Hide our 'new' editing widget

        newEditingWidget->hide();
    }
}

//==============================================================================

void RawSedmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    SEDMLEditingView::SedmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        if (mEditingWidget == editingWidget) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);

            editingWidget->saveSettings(settings);

            mNeedLoadingSettings = true;
            mEditingWidget = nullptr;
        }

        // Delete the editor and remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);
    }
}

//==============================================================================

void RawSedmlViewWidget::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so consider it reloaded, but only if it
    // has a corresponding widget that is invisible

    QWidget *crtWidget = widget(pFileName);

    if ((crtWidget != nullptr) && !crtWidget->isVisible()) {
        fileReloaded(pFileName);
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

    if (mEditingWidgets.contains(pFileName)) {
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

    SEDMLEditingView::SedmlEditingViewWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget != nullptr) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

EditorWidget::EditorWidget * RawSedmlViewWidget::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    SEDMLEditingView::SedmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    return (editingWidget != nullptr)?editingWidget->editorWidget():nullptr;
}

//==============================================================================

QWidget * RawSedmlViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

QList<QWidget *> RawSedmlViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget != nullptr) {
        return { mEditingWidget->editorWidget()->cursorPositionWidget(),
                 mEditingWidget->editorWidget()->editingModeWidget() };
    }

    return {};
}

//==============================================================================

void RawSedmlViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    SEDMLEditingView::SedmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if ((editingWidget != nullptr) && validate(pFileName, true)) {
        int line;
        int column;

        editingWidget->editorWidget()->cursorPosition(line, column);

        QDomDocument domDocument;

        domDocument.setContent(editingWidget->editorWidget()->contents());

        editingWidget->editorWidget()->setContents(Core::serialiseDomDocument(domDocument), false);
        editingWidget->editorWidget()->setCursorPosition(line, column);
    }
}

//==============================================================================

bool RawSedmlViewWidget::validate(const QString &pFileName, bool pOnlyErrors) const
{
    // Validate the given file

    SEDMLEditingView::SedmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget != nullptr) {
        // Clear the list of SED-ML issues

        EditorWidget::EditorListWidget *editorList = editingWidget->editorListWidget();

        editorList->clear();

        // Retrieve the list of SED-ML issues, if any

        SEDMLSupport::SedmlFile *sedmlFile = SEDMLSupport::SedmlFileManager::instance()->sedmlFile(pFileName);
        SEDMLSupport::SedmlFileIssues sedmlFileIssues;

        bool res = sedmlFile->isValid(editingWidget->editorWidget()->contents(), sedmlFileIssues);

        // Add whatever issue there may be to our list and select the first one
        // of them

        for (const auto &sedmlFileIssue : sedmlFileIssues) {
            if (   !pOnlyErrors
                || (sedmlFileIssue.type() == SEDMLSupport::SedmlFileIssue::Type::Error)) {
                EditorWidget::EditorListItem::Type issueType = EditorWidget::EditorListItem::Type::Fatal;

                switch (sedmlFileIssue.type()) {
                case SEDMLSupport::SedmlFileIssue::Type::Information:
                    issueType = EditorWidget::EditorListItem::Type::Information;

                    break;
                case SEDMLSupport::SedmlFileIssue::Type::Error:
                    issueType = EditorWidget::EditorListItem::Type::Error;

                    break;
                case SEDMLSupport::SedmlFileIssue::Type::Warning:
                    issueType = EditorWidget::EditorListItem::Type::Warning;

                    break;
                case SEDMLSupport::SedmlFileIssue::Type::Fatal:
                    issueType = EditorWidget::EditorListItem::Type::Fatal;

                    break;
                }

                editorList->addItem(issueType, sedmlFileIssue.line(), sedmlFileIssue.column(),
                                    sedmlFileIssue.message());
            }
        }

        editorList->selectFirstItem();

        return res;
    }

    // The file doesn't exist, so it can't be validated

    return false;
}

//==============================================================================

} // namespace RawSEDMLView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
