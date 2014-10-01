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
#include "editorwidget.h"
#include "filemanager.h"
#include "prettycellmlviewwidget.h"
#include "settings.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_prettycellmlviewwidget.h"

//==============================================================================

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
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *>())
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

void PrettyCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void PrettyCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void PrettyCellmlViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
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

    CoreCellMLEditing::CoreCellmlEditingWidget *oldEditingWidget = mEditingWidget;

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (!mEditingWidget) {
        // No editing widget exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        mEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(fileContents,
                                                                        !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                        new QsciLexerXML(this),
                                                                        parentWidget());

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, mEditingWidget);

        layout()->addWidget(mEditingWidget);
    }

    // Load our settings, if needed, or reset our editing widget using the old
    // one

    if (mNeedLoadingSettings) {
        QSettings settings(SettingsOrganization, SettingsApplication);

        settings.beginGroup(mSettingsGroup);
            mEditingWidget->loadSettings(&settings);
        settings.endGroup();

        mNeedLoadingSettings = false;
    } else {
        mEditingWidget->updateSettings(oldEditingWidget);
    }

    // Show/hide our editing widgets

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        if (editingWidget == mEditingWidget)
            // This is the editing widget we are after, so show it

            editingWidget->show();
        else
            // Not the editing widget we are after, so hide it

            editingWidget->hide();

    // Set our focus proxy to our 'new' editing widget and make sure that the
    // latter immediately gets the focus
    // Note: if we were not to immediately give the focus to our 'new' editing
    //       widget, then the central widget would give the focus to our 'old'
    //       editing widget (see CentralWidget::updateGui()), which is clearly
    //       not what we want...

    setFocusProxy(mEditingWidget->editor());

    mEditingWidget->editor()->setFocus();
}

//==============================================================================

void PrettyCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget  = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        if (editingWidget == mEditingWidget) {
            QSettings settings(SettingsOrganization, SettingsApplication);

            settings.beginGroup(mSettingsGroup);
                mEditingWidget->saveSettings(&settings);
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

Editor::EditorWidget * PrettyCellmlViewWidget::editor(const QString &pFileName) const
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

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
