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
// SED-ML Editing view widget
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "sedmleditingviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QScreen>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SEDMLEditingView {

//==============================================================================

SedmlEditingViewWidget::SedmlEditingViewWidget(const QString &pContents,
                                               bool pReadOnly,
                                               QsciLexer *pLexer,
                                               QWidget *pParent) :
    Core::SplitterWidget(pParent)
{
    // Set our orientation

    setOrientation(Qt::Vertical);

    // Keep track of our movement

    connect(this, &Core::SplitterWidget::splitterMoved,
            this, &SedmlEditingViewWidget::splitterMoved);

    // Create our editor and editor list widgets

    mEditorWidget = new EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorListWidget = new EditorWidget::EditorListWidget(this);

    mEditorWidget->setObjectName("EditorWidget");

    connect(mEditorListWidget, &EditorWidget::EditorListWidget::itemRequested,
            this, &SedmlEditingViewWidget::itemRequested);

    // Add the bordered editor and editor list widgets to ourselves

    addWidget(new Core::BorderedWidget(mEditorWidget, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorListWidget, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditorWidget);
}

//==============================================================================

static const char *SettingsSedmlEditingViewWidgetSizes = "SedmlEditingViewWidgetSizes";

//==============================================================================

void SedmlEditingViewWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve and set our sizes

    static const int AvailableGeometryHeight = qApp->primaryScreen()->availableGeometry().height();
    static const QVariantList DefaultEditingWidgetSizes = { 0.91*AvailableGeometryHeight,
                                                            0.09*AvailableGeometryHeight };

    mEditingWidgetSizes = qVariantListToIntList(pSettings.value(SettingsSedmlEditingViewWidgetSizes, DefaultEditingWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our editor widget's settings

    pSettings.beginGroup(mEditorWidget->objectName());
        mEditorWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void SedmlEditingViewWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of our sizes

    pSettings.setValue(SettingsSedmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our editor widget's settings

    pSettings.beginGroup(mEditorWidget->objectName());
        mEditorWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void SedmlEditingViewWidget::retranslateUi()
{
    // Retranslate our editor and editor list widgets

    mEditorWidget->retranslateUi();
    mEditorListWidget->retranslateUi();
}

//==============================================================================

void SedmlEditingViewWidget::updateSettings(SedmlEditingViewWidget *pEditingWidget)
{
    // Make sure that we are given another editing widget

    if ((pEditingWidget == nullptr) || (pEditingWidget == this)) {
        return;
    }

    // Update our sizes and editor widget's settings

    mEditingWidgetSizes = pEditingWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mEditorWidget->updateSettings(pEditingWidget->editorWidget());
}

//==============================================================================

EditorWidget::EditorWidget * SedmlEditingViewWidget::editorWidget() const
{
    // Return our editor widget

    return mEditorWidget;
}

//==============================================================================

EditorWidget::EditorListWidget * SedmlEditingViewWidget::editorListWidget() const
{
    // Return our editor list widget

    return mEditorListWidget;
}

//==============================================================================

QIntList SedmlEditingViewWidget::editingWidgetSizes() const
{
    // Return our editing widget sizes

    return mEditingWidgetSizes;
}

//==============================================================================

void SedmlEditingViewWidget::splitterMoved()
{
    // We have moved, so keep track of our new sizes

    mEditingWidgetSizes = sizes();
}

//==============================================================================

void SedmlEditingViewWidget::itemRequested(EditorWidget::EditorListItem *pItem)
{
    // Set our editor widget's cursor position to the line/column of the given
    // item and give our editor widget the focus so that we can see the exact
    // location of the item (otherwise it will be mEditorListWidget that will
    // have the focus since we just double-clicked on it)

    mEditorWidget->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditorWidget->setFocus();
}

//==============================================================================

} // namespace SEDMLEditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
