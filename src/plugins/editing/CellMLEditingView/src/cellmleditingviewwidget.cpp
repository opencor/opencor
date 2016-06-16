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
// CellML Editing view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmleditingviewwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "mathmlviewerwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLEditingView {

//==============================================================================

CellmlEditingViewWidget::CellmlEditingViewWidget(const QString &pContents,
                                                 const bool &pReadOnly,
                                                 QsciLexer *pLexer,
                                                 QWidget *pParent) :
    QSplitter(pParent),
    Core::CommonWidget(),
    mEditingWidgetSizes(QIntList())
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(splitterMoved()));

    // Create our viewer, editor and editor list

    mMathmlViewer = new MathMLViewerWidget::MathmlViewerWidget(this);
    mEditor = new EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorList = new EditorWidget::EditorListWidget(this);

    connect(mEditorList, SIGNAL(itemRequested(OpenCOR::EditorWidget::EditorListItem *)),
            this, SLOT(itemRequested(OpenCOR::EditorWidget::EditorListItem *)));

    // Add the bordered viewer, editor and editor list to ourselves

    addWidget(new Core::BorderedWidget(mMathmlViewer, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditor, true, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorList, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditor);
}

//==============================================================================

static const auto SettingsCellmlEditingViewWidgetSizes = QStringLiteral("CellmlEditingViewWidgetSizes");

//==============================================================================

void CellmlEditingViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes
    // Note #1: the viewer's and editor list's default height is 19% and 13%,
    //          respectively, of the desktop's height while that of the editor
    //          is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our widget, the viewer's and editor list's default height will
    //          effectively be less than 19% and 13%, respectively, of the
    //          desktop's height, but that doesn't matter at all...

    QVariantList defaultCellmlEditingViewWidgetSizes = QVariantList() << 0.19*qApp->desktop()->screenGeometry().height()
                                                                      << qApp->desktop()->screenGeometry().height()
                                                                      << 0.13*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsCellmlEditingViewWidgetSizes, defaultCellmlEditingViewWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our viewer's and editor's settings

    mMathmlViewer->loadSettings(pSettings);
    mEditor->loadSettings(pSettings);
}

//==============================================================================

void CellmlEditingViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsCellmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our viewer's and editor's settings

    mMathmlViewer->saveSettings(pSettings);
    mEditor->saveSettings(pSettings);
}

//==============================================================================

void CellmlEditingViewWidget::retranslateUi()
{
    // Retranslate our viewer, editor and editor list

    mMathmlViewer->retranslateUi();
    mEditor->retranslateUi();
    mEditorList->retranslateUi();
}

//==============================================================================

void CellmlEditingViewWidget::updateSettings(CellmlEditingViewWidget *pCellmlEditingViewWidget)
{
    // Make sure that we are given another editing widget

    if (!pCellmlEditingViewWidget || (pCellmlEditingViewWidget == this))
        return;

    // Update our sizes, viewer settings and editor settings

    mEditingWidgetSizes = pCellmlEditingViewWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mMathmlViewer->updateSettings(pCellmlEditingViewWidget->mathmlViewer());
    mEditor->updateSettings(pCellmlEditingViewWidget->editor());
}

//==============================================================================

MathMLViewerWidget::MathmlViewerWidget *CellmlEditingViewWidget::mathmlViewer() const
{
    // Return our MathML viewer

    return mMathmlViewer;
}

//==============================================================================

EditorWidget::EditorWidget * CellmlEditingViewWidget::editor() const
{
    // Return our editor

    return mEditor;
}

//==============================================================================

EditorWidget::EditorListWidget * CellmlEditingViewWidget::editorList() const
{
    // Return our editor list

    return mEditorList;
}

//==============================================================================

QIntList CellmlEditingViewWidget::editingWidgetSizes() const
{
    // Return our editing widget sizes

    return mEditingWidgetSizes;
}

//==============================================================================

void CellmlEditingViewWidget::splitterMoved()
{
    // We have moved, so keep track of our new sizes

    mEditingWidgetSizes = sizes();
}

//==============================================================================

void CellmlEditingViewWidget::itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem)
{
    // Set our editor's cursor position to the line/column of the given item and
    // give our editor the focus so that we can see the exact location of the
    // item (otherwise it will be mEditorList that will have the focus since we
    // just double-clicked on it)

    mEditor->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditor->setFocus();
}

//==============================================================================

}   // namespace CellMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
