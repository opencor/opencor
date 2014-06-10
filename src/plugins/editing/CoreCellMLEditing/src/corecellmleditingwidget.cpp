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
#include "cliutils.h"
#include "corecellmleditingwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "guiutils.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_corecellmleditingwidget.h"

//==============================================================================

#include <QDesktopWidget>
#include <QFileInfo>
#include <QListView>
#include <QSettings>
#include <QSplitter>

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
    mGui(new Ui::CoreCellmlEditingWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our bordered viewer

    mViewer = new Viewer::ViewerWidget(this);
    mBorderedViewer = new Core::BorderedWidget(mViewer,
                                               false, false, true, false);

    // Create our bordered editor

    mEditor = new Editor::EditorWidget(pContents, pReadOnly, pLexer, this);
    mBorderedEditor = new Core::BorderedWidget(mEditor,
                                               true, false, true, false);

    // Create our bordered list

    mList = new QListView(this);
    mBorderedList = new Core::BorderedWidget(mList,
                                             true, false, false, false);

#ifdef Q_OS_MAC
    mList->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our list
#endif
    mList->setFrameShape(QFrame::NoFrame);

    // Add the bordered viewer, editor and list to ourselves

    addWidget(mBorderedViewer);
    addWidget(mBorderedEditor);
    addWidget(mBorderedList);

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

void CoreCellmlEditingWidget::retranslateUi()
{
    // Retranslate our viewer and editor

    mViewer->retranslateUi();
    mEditor->retranslateUi();
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

QListView * CoreCellmlEditingWidget::list() const
{
    // Return our list

    return mList;
}

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
