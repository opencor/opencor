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
// Core CellML view widget
//==============================================================================

#include "borderedwidget.h"
#include "cliutils.h"
#include "corecellmlviewwidget.h"
#include "filemanager.h"
#include "guiutils.h"
#include "qscintillawidget.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_corecellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QSettings>
#include <QSplitter>

//==============================================================================

namespace OpenCOR {
namespace CoreCellMLEditing {

//==============================================================================

CoreCellmlViewWidget::CoreCellmlViewWidget(const QString &pFileName,
                                           QsciLexer *pLexer,
                                           QWidget *pParent) :
    QSplitter(pParent),
    mGui(new Ui::CoreCellmlViewWidget),
    mFileName(pFileName)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our bordered viewer

    mBorderedViewer = new Core::BorderedWidget(new Viewer::ViewerWidget(this),
                                               false, false, true, false);

    // Create and customise our bordered editor with the given lexer

    QScintillaSupport::QScintillaWidget *editor = new QScintillaSupport::QScintillaWidget(QString(),
                                                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                                          pLexer,
                                                                                          this);

    fileReloaded();

    mBorderedEditor = new Core::BorderedWidget(editor,
                                               true, false, false, false);

    // Keep track of changes to our editor's zoom level

    connect(editor, SIGNAL(SCN_ZOOM()),
            this, SIGNAL(editorZoomLevelChanged()));

    // Add the bordered viewer and editor to ourselves

    addWidget(mBorderedViewer);
    addWidget(mBorderedEditor);

    // Set our focus proxy to our editor

    setFocusProxy(editor);
}

//==============================================================================

CoreCellmlViewWidget::~CoreCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CoreCellmlViewWidget::fileReloaded()
{
    // The file has been reloaded, so update the contents of our editor

    QString fileContents;

    Core::readTextFromFile(mFileName, fileContents);

    mEditor->setContents(fileContents);
}

//==============================================================================

void CoreCellmlViewWidget::fileRenamed(const QString &pFileName)
{
    // The file has been renamed, so update ourselves

    mFileName = pFileName;
}

//==============================================================================

QScintillaSupport::QScintillaWidget * CoreCellmlViewWidget::editor() const
{
    // Return our editor

    return mEditor;
}

//==============================================================================

void CoreCellmlViewWidget::editorZoomLevelChanged()
{
    // One of our editors had its zoom level changed, so keep track of the new
    // zoom level

    emit editorZoomLevelChanged(qobject_cast<QScintillaSupport::QScintillaWidget *>(sender())->SendScintilla(QsciScintillaBase::SCI_GETZOOM));
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
