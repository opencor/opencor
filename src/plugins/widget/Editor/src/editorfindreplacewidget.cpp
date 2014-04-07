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
#include "guiinterface.h"

//==============================================================================

#include "ui_editorfindreplacewidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QKeyEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>

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

#ifdef Q_OS_MAC
    mGui->findEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);
    mGui->replaceEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above remove the focus border since it messes up the look of
    //       our edit widgets...
#endif

    // Create and handle our drop-down menu action

    mDropDownAction = GuiInterface::newAction(this, false, ":/qtCreator/src/plugins/coreplugin/images/magnifier.png", QKeySequence());

    mRegularExpressionAction = GuiInterface::newAction(this, true);
    mCaseSensitiveAction = GuiInterface::newAction(this, true);
    mWholeWordsOnlyAction = GuiInterface::newAction(this, true);

    QMenu *dropDownMenu = GuiInterface::newMenu(this);

    dropDownMenu->addAction(mRegularExpressionAction);
    dropDownMenu->addAction(mCaseSensitiveAction);
    dropDownMenu->addAction(mWholeWordsOnlyAction);

    mDropDownAction->setMenu(dropDownMenu);

    mGui->findEdit->addAction(mDropDownAction, QLineEdit::LeadingPosition);

    // Create and handle our clear text action

    mClearTextAction = GuiInterface::newAction(this, false, ":/qtCreator/src/plugins/coreplugin/images/editclear.png", QKeySequence());

    connect(mClearTextAction, SIGNAL(triggered()),
            mGui->findEdit, SLOT(clear()));

    // Make sure that we take as little vertical space as possible whilte as
    // much horizontal space as possible

    mGui->layout->setSizeConstraint(QLayout::SetMaximumSize);

    // Make our find edit widget our focus proxy

    setFocusProxy(mGui->findEdit);

    // Some connections for our find widget

    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SIGNAL(findTextChanged(const QString &)));
    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearTextAction(const QString &)));

    // Retranslate ourselves, so that we are properly initialised

    retranslateUi();
}

//==============================================================================

void EditorFindReplaceWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our actions

    GuiInterface::retranslateAction(mRegularExpressionAction, tr("Regular Expression"));
    GuiInterface::retranslateAction(mCaseSensitiveAction, tr("Case Sensitive"));
    GuiInterface::retranslateAction(mWholeWordsOnlyAction, tr("Whole Words Only"));

    GuiInterface::retranslateAction(mClearTextAction, tr("Clear the text"));
}

//==============================================================================

bool EditorFindReplaceWidget::regularExpression() const
{
    // Return whether we use a regular expression

    return mRegularExpressionAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::caseSensitive() const
{
    // Return whether the search is to be case sensitive

    return mCaseSensitiveAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::wholeWordsOnly() const
{
    // Return whether we search whole words only

    return mWholeWordsOnlyAction->isChecked();
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

void EditorFindReplaceWidget::updateClearTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our find widget
    // contains some text

    if (pText.isEmpty())
        mGui->findEdit->removeAction(mClearTextAction);
    else
        mGui->findEdit->addAction(mClearTextAction, QLineEdit::TrailingPosition);
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
