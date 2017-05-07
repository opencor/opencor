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
#include <QDesktopWidget>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace SEDMLEditingView {

//==============================================================================

SedmlEditingViewWidget::SedmlEditingViewWidget(const QString &pContents,
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

    // Create our editor and editor list widgets

    mEditorWidget = new EditorWidget::EditorWidget(pContents, pReadOnly, pLexer, this);
    mEditorListWidget = new EditorWidget::EditorListWidget(this);

    mEditorWidget->setObjectName("EditorWidget");

    connect(mEditorListWidget, SIGNAL(itemRequested(OpenCOR::EditorWidget::EditorListItem *)),
            this, SLOT(itemRequested(OpenCOR::EditorWidget::EditorListItem *)));

    // Add the bordered editor and editor list widgets to ourselves

    addWidget(new Core::BorderedWidget(mEditorWidget, false, false, true, false));
    addWidget(new Core::BorderedWidget(mEditorListWidget, true, false, false, false));

    // Set our focus proxy to our editor

    setFocusProxy(mEditorWidget);
}

//==============================================================================

static const auto SettingsSedmlEditingViewWidgetSizes = QStringLiteral("SedmlEditingViewWidgetSizes");

//==============================================================================

void SedmlEditingViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve and set our sizes
    // Note #1: the editor list widget's default height is 13% of the desktop's
    //          height while that of the editor widget is as big as it can be...
    // Note #2: because the editor widget's default height is much bigger than
    //          that of our widget, the editor list widget's default height will
    //          effectively be less than 13% of the desktop's height, but that
    //          doesn't matter at all...

    QVariantList defaultSedmlEditingViewWidgetSizes = QVariantList() << qApp->desktop()->screenGeometry().height()
                                                                     << 0.13*qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsSedmlEditingViewWidgetSizes, defaultSedmlEditingViewWidgetSizes).toList());

    setSizes(mEditingWidgetSizes);

    // Retrieve our editor widget's settings

    pSettings->beginGroup(mEditorWidget->objectName());
        mEditorWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void SedmlEditingViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our sizes

    pSettings->setValue(SettingsSedmlEditingViewWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));

    // Keep track of our editor widget's settings

    mEditorWidget->saveSettings(pSettings);
}

//==============================================================================

void SedmlEditingViewWidget::retranslateUi()
{
    // Retranslate our editor and editor list widgets

    mEditorWidget->retranslateUi();
    mEditorListWidget->retranslateUi();
}

//==============================================================================

void SedmlEditingViewWidget::updateSettings(SedmlEditingViewWidget *pSedmlEditingViewWidget)
{
    // Make sure that we are given another editing widget

    if (!pSedmlEditingViewWidget || (pSedmlEditingViewWidget == this))
        return;

    // Update our sizes and editor widget's settings

    mEditingWidgetSizes = pSedmlEditingViewWidget->editingWidgetSizes();

    setSizes(mEditingWidgetSizes);

    mEditorWidget->updateSettings(pSedmlEditingViewWidget->editorWidget());
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

void SedmlEditingViewWidget::itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem)
{
    // Set our editor widget's cursor position to the line/column of the given
    // item and give our editor widget the focus so that we can see the exact
    // location of the item (otherwise it will be mEditorListWidget that will
    // have the focus since we just double-clicked on it)

    mEditorWidget->setCursorPosition(pItem->line()-1, pItem->column()-1);
    mEditorWidget->setFocus();
}

//==============================================================================

}   // namespace SEDMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
