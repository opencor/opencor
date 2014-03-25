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
// Editor find/replace widget
//==============================================================================

#include "editorfindreplacewidget.h"

//==============================================================================

#include "ui_editorfindreplacewidget.h"

//==============================================================================

#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

EditorFindReplaceWidget::EditorFindReplaceWidget(QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::EditorFindReplaceWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our contents

    mFindLabel = new QLabel(this);
    mFindEdit = new QTextEdit(this);

    mReplaceLabel = new QLabel(this);
    mReplaceEdit = new QTextEdit(this);

    mFindEdit->setTabChangesFocus(true);
    mReplaceEdit->setTabChangesFocus(true);

    // Add our contents to our layout

    mGui->layout->addWidget(mFindLabel, 0, 0);
    mGui->layout->addWidget(mFindEdit, 0, 1);

    mGui->layout->addWidget(mReplaceLabel, 1, 0);
    mGui->layout->addWidget(mReplaceEdit, 1, 1);

    // Make our find edit widget our focus proxy

    setFocusProxy(mFindEdit);

    // Retranslate ourselves, so our various widgets are properly set

    retranslateUi();
}

//==============================================================================

void EditorFindReplaceWidget::retranslateUi()
{
    // Retranslate our find/replace widget

    mFindLabel->setText(tr("Find:"));
    mReplaceLabel->setText(tr("Replace:"));
}

//==============================================================================

void EditorFindReplaceWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Let people know that a key has been pressed

    bool handled = false;

    emit keyPressed(pEvent, handled);

    // Carry on as normal, if the event wasn't handled

    if (handled)
        // Accept the event

        pEvent->accept();
    else
        // Default handling of the event

        QWidget::keyPressEvent(pEvent);
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
