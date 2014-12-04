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
// Core CellML editing widget
//==============================================================================

#include "borderedwidget.h"
#include "corecellmleditingwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_corecellmleditingwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QListView>
#include <QSettings>
#include <QSplitter>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

CoreCellmlEditingWidget::CoreCellmlEditingWidget(const QString &pContents,
                                                 const bool &pReadOnly,
                                                 QsciLexer *pLexer,
                                                 QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(pParent),
    mGui(new Ui::CoreCellmlEditingWidget),
    mEditingWidgetSizes(QIntList())
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(splitterMoved()));

    // Create our viewer, editor and editor list

    mViewer = new Viewer::ViewerWidget(this);
    mEditor = new Editor::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorList = new EditorList::EditorListWidget(this);

    connect(mEditorList, SIGNAL(itemRequested(EditorList::EditorListItem *)),
            this, SLOT(itemRequested(EditorList::EditorListItem *)));

    // Add the bordered viewer, editor and editor list to ourselves

    addWidget(new Core::BorderedWidget(mViewer, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditor, true, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorList, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditor);
}

//==============================================================================

CoreCellmlEditingWidget::~CoreCellmlEditingWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

static const auto SettingsCoreCellmlEditingWidgetSizes = QStringLiteral("EditingWidgetSizes");

//==============================================================================

void CoreCellmlEditingWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes
    // Note #1: the viewer's and editor list's default height is 19% and 13%,
    //          respectively, of the desktop's height while that of the editor
    //          is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our widget, the viewer's and editor list's default height will
    //          effectively be less than 19% and 13%, respectively, of the
    //          desktop's height, but that doesn't matter at all...

    QVariantList defaultEditingWidgetSizes = QVariantList() << 0.19*qApp->desktop()->screenGeometry().height()
                                                            << qApp->desktop()->screenGeometry().height()
                                                            << 0.13*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsCoreCellmlEditingWidgetSizes, defaultEditingWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our viewer's and editor's settings

    mViewer->loadSettings(pSettings);
    mEditor->loadSettings(pSettings);
}

//==============================================================================

void CoreCellmlEditingWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsCoreCellmlEditingWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our viewer's and editor's settings

    mViewer->saveSettings(pSettings);
    mEditor->saveSettings(pSettings);
}

//==============================================================================

void CoreCellmlEditingWidget::retranslateUi()
{
    // Retranslate our viewer, editor and editor list

    mViewer->retranslateUi();
    mEditor->retranslateUi();
    mEditorList->retranslateUi();
}

//==============================================================================

void CoreCellmlEditingWidget::updateSettings(CoreCellmlEditingWidget *pCoreCellmlEditingWidget)
{
    // Make sure that we are given another editing widget

    if (!pCoreCellmlEditingWidget || (pCoreCellmlEditingWidget == this))
        return;

    // Update our sizes, viewer settings and editor settings

    mEditingWidgetSizes = pCoreCellmlEditingWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mViewer->updateSettings(pCoreCellmlEditingWidget->viewer());
    mEditor->updateSettings(pCoreCellmlEditingWidget->editor());
}

//==============================================================================

Viewer::ViewerWidget * CoreCellmlEditingWidget::viewer() const
{
    // Return our viewer

    return mViewer;
}

//==============================================================================

Editor::EditorWidget * CoreCellmlEditingWidget::editor() const
{
    // Return our editor

    return mEditor;
}

//==============================================================================

EditorList::EditorListWidget * CoreCellmlEditingWidget::editorList() const
{
    // Return our editor list

    return mEditorList;
}

//==============================================================================

QIntList CoreCellmlEditingWidget::editingWidgetSizes() const
{
    // Return our editing widget sizes

    return mEditingWidgetSizes;
}

//==============================================================================

void CoreCellmlEditingWidget::splitterMoved()
{
    // We have moved, so keep track of our new sizes

    mEditingWidgetSizes = sizes();
}

//==============================================================================

void CoreCellmlEditingWidget::itemRequested(EditorList::EditorListItem *pItem)
{
    // Set our editor's cursor position to the line/column of the given item and
    // give our editor the focus so that we can see the exact location of the
    // item (otherwise it will be mEditorList that will have the focus since we
    // just double-clicked on it)

    mEditor->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditor->setFocus();
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
