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
    Core::SplitterWidget(pParent),
    mEditingWidgetSizes(QIntList())
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement

    connect(this, SIGNAL(splitterMoved(int, int)),
            this, SLOT(splitterMoved()));

    // Create our MathML viewer, editor and editor list widgets

    mMathmlViewerWidget = new MathMLViewerWidget::MathmlViewerWidget(this);
    mEditorWidget = new EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorListWidget = new EditorWidget::EditorListWidget(this);

    mMathmlViewerWidget->setObjectName("MathmlViewerWidget");
    mEditorWidget->setObjectName("mEditorWidget");

    connect(mEditorListWidget, SIGNAL(itemRequested(OpenCOR::EditorWidget::EditorListItem *)),
            this, SLOT(itemRequested(OpenCOR::EditorWidget::EditorListItem *)));

    // Add the bordered MathML viewer, editor and editor list widgets to
    // ourselves

    addWidget(new Core::BorderedWidget(mMathmlViewerWidget, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorWidget, true, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorListWidget, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditorWidget);
}

//==============================================================================

static const auto SettingsCellmlEditingViewWidgetSizes = QStringLiteral("CellmlEditingViewWidgetSizes");

//==============================================================================

void CellmlEditingViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes
    // Note #1: the MathML viewer and editor list widgets' default height is 19%
    //          and 13%, respectively, of the desktop's height while that of the
    //          editor widget is as big as it can be...
    // Note #2: because the editor widget's default height is much bigger than
    //          that of our widget, the MathML viewer and editor list widgets'
    //          default height will effectively be less than 19% and 13%,
    //          respectively, of the desktop's height, but that doesn't matter
    //          at all...

    QVariantList defaultCellmlEditingViewWidgetSizes = QVariantList() << 0.19*qApp->desktop()->screenGeometry().height()
                                                                      << qApp->desktop()->screenGeometry().height()
                                                                      << 0.13*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsCellmlEditingViewWidgetSizes, defaultCellmlEditingViewWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our MathML viewer and editor widgets' settings

    pSettings->beginGroup(mMathmlViewerWidget->objectName());
        mMathmlViewerWidget->loadSettings(pSettings);
    pSettings->endGroup();

    pSettings->beginGroup(mEditorWidget->objectName());
        mEditorWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CellmlEditingViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsCellmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our MathML viewer and editor widgets' settings

    mMathmlViewerWidget->saveSettings(pSettings);
    mEditorWidget->saveSettings(pSettings);
}

//==============================================================================

void CellmlEditingViewWidget::retranslateUi()
{
    // Retranslate our MathML viewer, editor and editor list widgets

    mMathmlViewerWidget->retranslateUi();
    mEditorWidget->retranslateUi();
    mEditorListWidget->retranslateUi();
}

//==============================================================================

void CellmlEditingViewWidget::updateSettings(CellmlEditingViewWidget *pCellmlEditingViewWidget)
{
    // Make sure that we are given another editing widget

    if (!pCellmlEditingViewWidget || (pCellmlEditingViewWidget == this))
        return;

    // Update our sizes, MathML viewer and editor widgets' settings

    mEditingWidgetSizes = pCellmlEditingViewWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mMathmlViewerWidget->updateSettings(pCellmlEditingViewWidget->mathmlViewer());
    mEditorWidget->updateSettings(pCellmlEditingViewWidget->editorWidget());
}

//==============================================================================

MathMLViewerWidget::MathmlViewerWidget *CellmlEditingViewWidget::mathmlViewer() const
{
    // Return our MathML viewer widget

    return mMathmlViewerWidget;
}

//==============================================================================

EditorWidget::EditorWidget * CellmlEditingViewWidget::editorWidget() const
{
    // Return our editor widget

    return mEditorWidget;
}

//==============================================================================

EditorWidget::EditorListWidget * CellmlEditingViewWidget::editorListWidget() const
{
    // Return our editor list widget

    return mEditorListWidget;
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
    // Set our editor widget's cursor position to the line/column of the given
    // item and give our editor widget the focus so that we can see the exact
    // location of the item (otherwise it will be mEditorListWidget that will
    // have the focus since we just double-clicked on it)

    mEditorWidget->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditorWidget->setFocus();
}

//==============================================================================

}   // namespace CellMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
