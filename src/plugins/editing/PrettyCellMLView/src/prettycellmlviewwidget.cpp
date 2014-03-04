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
// Pretty CellML view widget
//==============================================================================

#include "corecellmleditingwidget.h"
#include "filemanager.h"
#include "qscintillawidget.h"
#include "prettycellmlviewwidget.h"

//==============================================================================

#include "ui_prettycellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QVariant>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellmlViewWidget::PrettyCellmlViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::PrettyCellmlViewWidget),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *>()),
    mEditingWidgetSizes(QIntList()),
    mEditorZoomLevel(0)
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

PrettyCellmlViewWidget::~PrettyCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

static const auto SettingsEditingWidgetSizes = QStringLiteral("EditingWidgetSizes");
static const auto SettingsEditorZoomLevel    = QStringLiteral("EditorZoomLevel");

//==============================================================================

void PrettyCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the editing widget's sizes and the editor's zoom level
    // Note #1: the viewer's default height is 19% of the desktop's height while
    //          that of the editor is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our raw CellML view widget, the viewer's default height will
    //          effectively be less than 19% of the desktop's height, but that
    //          doesn't matter at all...

    QVariantList defaultEditingWidgetSizes = QVariantList() << 0.19*qApp->desktop()->screenGeometry().height()
                                                            << qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsEditingWidgetSizes, defaultEditingWidgetSizes).toList());
    mEditorZoomLevel = pSettings->value(SettingsEditorZoomLevel, 0).toInt();
}

//==============================================================================

void PrettyCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the editing widget's sizes and the editor's zoom level

    pSettings->setValue(SettingsEditingWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));
    pSettings->setValue(SettingsEditorZoomLevel, mEditorZoomLevel);
}

//==============================================================================

bool PrettyCellmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file, i.e. whether we have an
    // editing widget for it

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

void PrettyCellmlViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (!mEditingWidget) {
        // No editing widget exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        mEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(fileContents,
                                                                        !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                        new QsciLexerXML(this),
                                                                        parentWidget());

        // Keep track of our editing widget's sizes when moving the splitter and
        // of changes to our editor's zoom level

        connect(mEditingWidget, SIGNAL(splitterMoved(int, int)),
                this, SLOT(splitterMoved()));

        connect(mEditingWidget->editor(), SIGNAL(SCN_ZOOM()),
                this, SLOT(editorZoomLevelChanged()));

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, mEditingWidget);

        layout()->addWidget(mEditingWidget);
    }

    // Show/hide our editing widgets and adjust our sizes

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        if (editingWidget == mEditingWidget) {
            // This is the editing widget we are after, so show it and update
            // its size and zoom level

            editingWidget->setSizes(mEditingWidgetSizes);
            editingWidget->editor()->zoomTo(mEditorZoomLevel);

            editingWidget->show();
        } else {
            // Not the editing widget we are after, so hide it

            editingWidget->hide();
        }

    // Set our focus proxy to our 'new' editing widget and make sure that the
    // latter immediately gets the focus
    // Note: if we were not to immediately give our 'new' editing widget the
    //       focus, then the central widget would give the focus to our 'old'
    //       editing widget (see CentralWidget::updateGui()), so...

    setFocusProxy(mEditingWidget->editor());

    mEditingWidget->editor()->setFocus();
}

//==============================================================================

void PrettyCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget  = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editor for the given file name, so delete it and remove
        // it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget)
            mEditingWidget = 0;
    }
}

//==============================================================================

void PrettyCellmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

void PrettyCellmlViewWidget::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

QScintillaSupport::QScintillaWidget * PrettyCellmlViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?editingWidget->editor():0;
}

//==============================================================================

QList<QWidget *> PrettyCellmlViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget)
        return QList<QWidget *>() << mEditingWidget->editor()->cursorPositionWidget()
                                  << mEditingWidget->editor()->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

void PrettyCellmlViewWidget::splitterMoved()
{
    // The splitter has moved, so keep track of the editing widget's sizes

    mEditingWidgetSizes = qobject_cast<CoreCellMLEditing::CoreCellmlEditingWidget *>(sender())->sizes();
}

//==============================================================================

void PrettyCellmlViewWidget::editorZoomLevelChanged()
{
    // One of our editors had its zoom level changed, so keep track of the new
    // zoom level

    mEditorZoomLevel = qobject_cast<QScintillaSupport::QScintillaWidget *>(sender())->SendScintilla(QsciScintillaBase::SCI_GETZOOM);
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
