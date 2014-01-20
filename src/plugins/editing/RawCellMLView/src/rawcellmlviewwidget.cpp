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
// Raw CellML view widget
//==============================================================================

#include "borderedwidget.h"
#include "filemanager.h"
#include "guiutils.h"
#include "qscintillawidget.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QSettings>
#include <QSplitter>
#include <QTextStream>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

RawCellmlViewWidget::RawCellmlViewWidget(QWidget *pParent) :
    QSplitter(pParent),
    CommonWidget(pParent),
    mGui(new Ui::RawCellmlViewWidget),
    mBorderedEditor(0),
    mBorderedEditors(QMap<QString, Core::BorderedWidget *>()),
    mBorderedViewerHeight(0),
    mBorderedEditorHeight(0),
    mEditorZoomLevel(0)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Keep track of our sizes when moving the splitter

    connect(this, SIGNAL(splitterMoved(int,int)),
            this, SLOT(splitterMoved()));

    // Create our viewer

    mViewer = new Viewer::ViewerWidget(this);
    mBorderedViewer = new Core::BorderedWidget(mViewer,
                                               false, false, true, false);

    // Add the bordered viewer to ourselves

    addWidget(mBorderedViewer);
}

//==============================================================================

RawCellmlViewWidget::~RawCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

static const auto SettingsBorderedViewerHeight = QStringLiteral("BorderedViewerHeight");
static const auto SettingsBorderedEditorHeight = QStringLiteral("BorderedEditorHeight");
static const auto SettingsEditorZoomLevel      = QStringLiteral("EditorZoomLevel");

//==============================================================================

void RawCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the bordered viewer's and editor's height, as well as the
    // editor's zoom level
    // Note #1: the viewer's default height is 19% of the desktop's height while
    //          that of the editor is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our raw CellML view widget, the viewer's default height will
    //          effectively be less than 19% of the desktop's height, but that
    //          doesn't matter at all...

    mBorderedViewerHeight = pSettings->value(SettingsBorderedViewerHeight,
                                             0.19*qApp->desktop()->screenGeometry().height()).toInt();
    mBorderedEditorHeight = pSettings->value(SettingsBorderedEditorHeight,
                                             qApp->desktop()->screenGeometry().height()).toInt();

    mEditorZoomLevel = pSettings->value(SettingsEditorZoomLevel, 0).toInt();
}

//==============================================================================

void RawCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the bordered viewer's and editor's height, as well as the
    // editor's zoom level
    // Note #1: we must also keep track of the editor's height because when
    //          loading our settings (see above), the widget doesn't yet have a
    //          'proper' height, so we couldn't simply assume that the editor's
    //          initial height is this widget's height minus the viewer's
    //          initial height, so...
    // Note #2: we rely on mBorderedViewerHeight and mBorderedEditorHeight
    //          rather than directly calling the height() method of the viewer
    //          and of the editor, respectively since it may happen that the
    //          user exits OpenCOR without ever having switched to the raw
    //          CellML view, in which case we couldn't retrieve the viewer and
    //          editor's height which in turn would result in OpenCOR crashing,
    //          so...

    pSettings->setValue(SettingsBorderedViewerHeight, mBorderedViewerHeight);
    pSettings->setValue(SettingsBorderedEditorHeight, mBorderedEditorHeight);

    pSettings->setValue(SettingsEditorZoomLevel, mEditorZoomLevel);
}

//==============================================================================

bool RawCellmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given CellML file, i.e. whether we have
    // an editor for it

    return mBorderedEditors.value(pFileName);
}

//==============================================================================

void RawCellmlViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editor associated with the given CellML file, if any

    mBorderedEditor = mBorderedEditors.value(pFileName);

    if (!mBorderedEditor) {
        // No editor exists for the given CellML file, so create and set up a
        // Scintilla editor with an XML lexer associated with it

        QFile file(pFileName);
        QString fileContents = QString();

        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            fileContents = QTextStream(&file).readAll();

            file.close();
        }

        QScintillaSupport::QScintillaWidget *editor = new QScintillaSupport::QScintillaWidget(fileContents,
                                                                                              !Core::FileManager::instance()->isLocked(pFileName),
                                                                                              new QsciLexerXML(this),
                                                                                              parentWidget());

        mBorderedEditor = new Core::BorderedWidget(editor,
                                                   true, false, false, false);

        // Keep track of changes to our editor's zoom level

        connect(editor, SIGNAL(SCN_ZOOM()),
                this, SLOT(editorZoomLevelChanged()));

        // Keep track of our bordered editor and add it to ourselves

        mBorderedEditors.insert(pFileName, mBorderedEditor);

        addWidget(mBorderedEditor);
    }

    // Show/hide our bordered editors and adjust our sizes

    QList<int> newSizes = QList<int>() << mBorderedViewerHeight;

    for (int i = 1, iMax = count(); i < iMax; ++i) {
        Core::BorderedWidget *borderedEditor = qobject_cast<Core::BorderedWidget *>(widget(i));

        if (borderedEditor == mBorderedEditor) {
            // This is the editor we are after, so show it, set/update its size
            // and zoom level

            borderedEditor->show();

            newSizes << mBorderedEditorHeight;

            qobject_cast<QScintillaSupport::QScintillaWidget *>(borderedEditor->widget())->zoomTo(mEditorZoomLevel);
        } else {
            // Not the editor we are after, so hide it and set its size
            // Note: theoretically speaking, we could set its size to whatever
            //       value we want since it's anyway hidden...

            borderedEditor->hide();

            newSizes << 0;
        }
    }

    setSizes(newSizes);

    // Set the raw CellML view widget's focus proxy to our 'new' editor and
    // make sure that it immediately gets the focus
    // Note: if we were not to immediately give our 'new' editor the focus,
    //       then the central widget would give the focus to our 'old' editor
    //       (see CentralWidget::updateGui()), so...

    setFocusProxy(mBorderedEditor->widget());

    mBorderedEditor->widget()->setFocus();
}

//==============================================================================

void RawCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the bordered editor, should there be one for the given CellML file

    Core::BorderedWidget *borderedEditor  = mBorderedEditors.value(pFileName);

    if (borderedEditor) {
        // There is a bordered editor for the given file name, so delete it and
        // remove it from our list

        delete borderedEditor;

        mBorderedEditors.remove(pFileName);

        // Reset our memory of the current bordered editor

        mBorderedEditor = 0;
    }
}

//==============================================================================

void RawCellmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
    }
}

//==============================================================================

QScintillaSupport::QScintillaWidget * RawCellmlViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    Core::BorderedWidget *borderedEditor  = mBorderedEditors.value(pFileName);

    return borderedEditor?qobject_cast<QScintillaSupport::QScintillaWidget *>(borderedEditor->widget()):0;
}

//==============================================================================

void RawCellmlViewWidget::editorZoomLevelChanged()
{
    // One of our view widgets had its zoom level changed, so keep track of the
    // new zoom level

    mEditorZoomLevel = qobject_cast<QScintillaSupport::QScintillaWidget *>(sender())->SendScintilla(QsciScintillaBase::SCI_GETZOOM);
}

//==============================================================================

void RawCellmlViewWidget::splitterMoved()
{
    // The splitter has moved, so keep track of the viewer and editor's new
    // height

    mBorderedViewerHeight = mBorderedViewer->height();
    mBorderedEditorHeight = mBorderedEditor->height();
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
