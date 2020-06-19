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
// CellML Editing view widget
//==============================================================================

#include "borderedwidget.h"
#include "cellmleditingviewwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "mathmlviewerwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopServices>
#include <QScreen>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace CellMLEditingView {

//==============================================================================

CellmlEditingViewWidgetEditorWidget::CellmlEditingViewWidgetEditorWidget(const QString &pContents,
                                                                         bool pReadOnly,
                                                                         QsciLexer *pLexer,
                                                                         CellmlEditingViewWidget *pParent) :
    EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, pParent),
    mOwner(pParent)
{
}

//==============================================================================

bool CellmlEditingViewWidgetEditorWidget::handleEditorKeyPressEvent(QKeyEvent *pEvent)
{
    // Give our owner a chance to handle our editor's key press event, if it
    // wants

    return mOwner->handleEditorKeyPressEvent(pEvent);
}

//==============================================================================

CellmlEditingViewWidget::CellmlEditingViewWidget(const QString &pContents,
                                                 bool pReadOnly,
                                                 QsciLexer *pLexer,
                                                 QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement

    connect(this, &Core::SplitterWidget::splitterMoved,
            this, &CellmlEditingViewWidget::splitterMoved);

    // Create our MathML viewer, editor and editor list widgets

    mMathmlViewerWidget = new MathMLViewerWidget::MathmlViewerWidget(this);
    mEditorWidget = new CellmlEditingViewWidgetEditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorListWidget = new EditorWidget::EditorListWidget(this);

    mMathmlViewerWidget->setObjectName("MathmlViewerWidget");
    mEditorWidget->setObjectName("EditorWidget");

    connect(mEditorListWidget, &EditorWidget::EditorListWidget::itemRequested,
            this, &CellmlEditingViewWidget::itemRequested);

    // Add the bordered MathML viewer, editor and editor list widgets to
    // ourselves

    addWidget(new Core::BorderedWidget(mMathmlViewerWidget, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorWidget, true, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorListWidget, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditorWidget);
}

//==============================================================================

static const char *SettingsCellmlEditingViewWidgetSizes = "CellmlEditingViewWidgetSizes";

//==============================================================================

void CellmlEditingViewWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve and set our sizes

    static const int AvailableGeometryHeight = qApp->primaryScreen()->availableGeometry().height();
    static const QVariantList DefaultEditingWidgetSizes = { 0.15*AvailableGeometryHeight,
                                                            0.76*AvailableGeometryHeight,
                                                            0.09*AvailableGeometryHeight };

    mEditingWidgetSizes = qVariantListToIntList(pSettings.value(SettingsCellmlEditingViewWidgetSizes, DefaultEditingWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our MathML viewer and editor widgets' settings

    pSettings.beginGroup(mMathmlViewerWidget->objectName());
        mMathmlViewerWidget->loadSettings(pSettings);
    pSettings.endGroup();

    pSettings.beginGroup(mEditorWidget->objectName());
        mEditorWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CellmlEditingViewWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of our sizes

    pSettings.setValue(SettingsCellmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our MathML viewer and editor widgets' settings

    pSettings.beginGroup(mMathmlViewerWidget->objectName());
        mMathmlViewerWidget->saveSettings(pSettings);
    pSettings.endGroup();

    pSettings.beginGroup(mEditorWidget->objectName());
        mEditorWidget->saveSettings(pSettings);
    pSettings.endGroup();
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

void CellmlEditingViewWidget::updateSettings(CellmlEditingViewWidget *pEditingWidget)
{
    // Make sure that we are given another editing widget

    if ((pEditingWidget == nullptr) || (pEditingWidget == this)) {
        return;
    }

    // Update our sizes, MathML viewer and editor widgets' settings

    mEditingWidgetSizes = pEditingWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mMathmlViewerWidget->updateSettings(pEditingWidget->mathmlViewer());
    mEditorWidget->updateSettings(pEditingWidget->editorWidget());
}

//==============================================================================

bool CellmlEditingViewWidget::handleEditorKeyPressEvent(QKeyEvent *pEvent)
{
    Q_UNUSED(pEvent)

    // By default, we don't handle our editor's key press event

    return false;
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

void CellmlEditingViewWidget::itemRequested(EditorWidget::EditorListItem *pItem)
{
    // Check whether the given item is located in the current file or in a
    // different one

    if (pItem->fileName().isEmpty()) {
        // The given item is located in the current file, so set our editor
        // widget's cursor position to its line/column and give our editor
        // widget the focus so that we can see the exact location of the item
        // (otherwise it will be mEditorListWidget that will have the focus
        // since we just double-clicked on it)

        mEditorWidget->setCursorPosition(pItem->line()-1, pItem->column()-1);
        mEditorWidget->setFocus();
    } else {
        // The given item is located in a different file, so open that file, and
        // ask set our cursor position to its line/column
        // Note: we retrieve the current view plugin's name before opening the
        //       file to prevent any potential issue. I don't think it could
        //       happen, but say that we need to open a CellML file. This means
        //       that the CellML Text view will, by default, be selected, and it
        //       might be selected before we query the current view plugin's
        //       name, which means that the wrong view might be selected in the
        //       end, if we were not originally using the CellML Text view (e.g.
        //       if we were using the Raw CellML view)...

        QString currentViewPluginName = Core::currentViewPlugin()->name();

        QDesktopServices::openUrl("opencor://openFile/"+pItem->fileName());
        QDesktopServices::openUrl("opencor://Core.selectView/"+currentViewPluginName);
        QDesktopServices::openUrl(QString("opencor://EditingView.setCursorPosition/%1|%2").arg(pItem->line())
                                                                                          .arg(pItem->column()));
    }
}

//==============================================================================

} // namespace CellMLEditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
