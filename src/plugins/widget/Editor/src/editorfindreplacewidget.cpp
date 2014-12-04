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

#include "coreguiutils.h"
#include "editorfindreplacewidget.h"
#include "i18ninterface.h"

//==============================================================================

#include "ui_editorfindreplacewidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QKeyEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

EditorFindReplaceWidget::EditorFindReplaceWidget(QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::EditorFindReplaceWidget),
    mActive(false)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->findEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->replaceEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above remove the focus border since it messes up the look of
    //       our edit widgets...
#endif

    // Create and handle our drop-down menu action

    mDropDownAction = new QAction(this);

    mCaseSensitiveAction = Core::newAction(true, this);
    mWholeWordsOnlyAction = Core::newAction(true, this);
    mRegularExpressionAction = Core::newAction(true, this);

    QMenu *dropDownMenu = new QMenu(this);

    dropDownMenu->addAction(mCaseSensitiveAction);
    dropDownMenu->addAction(mWholeWordsOnlyAction);
    dropDownMenu->addAction(mRegularExpressionAction);

    mDropDownAction->setMenu(dropDownMenu);

    mGui->findEdit->addAction(mDropDownAction, QLineEdit::LeadingPosition);

    connect(mCaseSensitiveAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mWholeWordsOnlyAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));
    connect(mRegularExpressionAction, SIGNAL(toggled(bool)),
            this, SLOT(searchOptionChanged()));

    // Create and handle our clear find and replace text actions

    mClearFindTextAction = Core::newAction(QIcon(":/qtCreator/src/plugins/coreplugin/images/editclear.png"), this);
    mClearReplaceTextAction = Core::newAction(QIcon(":/qtCreator/src/plugins/coreplugin/images/editclear.png"), this);

    connect(mClearFindTextAction, SIGNAL(triggered(bool)),
            mGui->findEdit, SLOT(clear()));

    connect(mClearReplaceTextAction, SIGNAL(triggered(bool)),
            mGui->replaceEdit, SLOT(clear()));

    // Make our find edit widget our focus proxy

    setFocusProxy(mGui->findEdit);

    // Some connections for our find-related widgets

    connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearFindTextAction(const QString &)));

    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->findPreviousButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->findNextButton, SLOT(setEnabled(bool)));

    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceAndFindButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canFindReplace(const bool &)),
            mGui->replaceAllButton, SLOT(setEnabled(bool)));

    // A connection for our replace widget

    connect(mGui->replaceEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateClearReplaceTextAction(const QString &)));

    // A few more things , so that we are properly initialised

    retranslateUi();

    searchOptionChanged();

    setActive(true);

    updateStyleSheet();
    updateHeight();
    // Note: just to be safe, we update our height after updating our style
    //       sheet since we change the size of our tool buttons...

    mGui->findPreviousButton->setEnabled(false);
    mGui->findNextButton->setEnabled(false);

    mGui->replaceButton->setEnabled(false);
    mGui->replaceAndFindButton->setEnabled(false);
    mGui->replaceAllButton->setEnabled(false);
}

//==============================================================================

void EditorFindReplaceWidget::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);

    // Retranslate our actions

    I18nInterface::retranslateAction(mCaseSensitiveAction, tr("Case Sensitive"),
                                     tr("The search is case sensitive"));
    I18nInterface::retranslateAction(mWholeWordsOnlyAction, tr("Whole Words Only"),
                                     tr("The search is done on whole words only"));
    I18nInterface::retranslateAction(mRegularExpressionAction, tr("Regular Expression"),
                                     tr("The search uses a regular expression"));

    I18nInterface::retranslateAction(mClearFindTextAction, tr("Clear Text"),
                                     tr("Clear the text"));
    I18nInterface::retranslateAction(mClearReplaceTextAction, tr("Clear Text"),
                                     tr("Clear the text"));
}

//==============================================================================

bool EditorFindReplaceWidget::isCaseSensitive() const
{
    // Return whether the search is to be case sensitive

    return mCaseSensitiveAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::searchWholeWordsOnly() const
{
    // Return whether we search whole words only

    return mWholeWordsOnlyAction->isChecked();
}

//==============================================================================

bool EditorFindReplaceWidget::useRegularExpression() const
{
    // Return whether we use a regular expression

    return mRegularExpressionAction->isChecked();
}

//==============================================================================

void EditorFindReplaceWidget::setReadOnly(const bool &pReadOnly)
{
    // Show/hide our replace-related widgets based on whether we are in
    // read-only mode

    Core::showEnableWidget(mGui->replaceLabel, !pReadOnly);
    Core::showEnableWidget(mGui->replaceEdit, !pReadOnly);
    Core::showEnableWidget(mGui->replaceButton, !pReadOnly);
    Core::showEnableWidget(mGui->replaceAndFindButton, !pReadOnly);
    Core::showEnableWidget(mGui->replaceAllButton, !pReadOnly);

    // Enable/disable our find spacer

    mGui->findLayout->setStretch(2, !pReadOnly);

    // Disable our replace-related buttons, if needed

    if (findText().isEmpty()) {
        mGui->replaceButton->setEnabled(false);
        mGui->replaceAndFindButton->setEnabled(false);
        mGui->replaceAllButton->setEnabled(false);
    }

    // Update our height

    updateHeight();
}

//==============================================================================

bool EditorFindReplaceWidget::isFindPreviousNextAvailable() const
{
    // Return whether the find previous/next actions are available

    return !findText().isEmpty();
}

//==============================================================================

void EditorFindReplaceWidget::selectFindText() const
{
    // Select our find text

    mGui->findEdit->selectAll();
}

//==============================================================================

QString EditorFindReplaceWidget::findText() const
{
    // Return our find text

    return mGui->findEdit->text();
}

//==============================================================================

void EditorFindReplaceWidget::setFindText(const QString &pFindText)
{
    // Set our find text and select it

    mGui->findEdit->setText(pFindText);

    selectFindText();
}

//==============================================================================

QString EditorFindReplaceWidget::replaceText() const
{
    // Return our replace text

    return mGui->replaceEdit->text();
}

//==============================================================================

bool EditorFindReplaceWidget::findEditHasFocus() const
{
    // Return whether our find edit has the focus

    return mGui->findEdit->hasFocus();
}

//==============================================================================

bool EditorFindReplaceWidget::replaceEditHasFocus() const
{
    // Return whether our replace edit has the focus

    return mGui->replaceEdit->hasFocus();
}

//==============================================================================

bool EditorFindReplaceWidget::isActive() const
{
    // Return whether we are active

    return mActive;
}

//==============================================================================

void EditorFindReplaceWidget::setActive(const bool &pActive)
{
    if (pActive == mActive)
        return;

    // Set our active state

    mActive = pActive;

    if (pActive)
        connect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
                this, SIGNAL(findTextChanged(const QString &)));
    else
        disconnect(mGui->findEdit, SIGNAL(textChanged(const QString &)),
                   this, SIGNAL(findTextChanged(const QString &)));
}

//==============================================================================

void EditorFindReplaceWidget::updateFrom(EditorFindReplaceWidget *pFindReplace)
{
    // Update our find and replace texts from the given find/replace widget

    mGui->findEdit->setText(pFindReplace->findText());
    mGui->replaceEdit->setText(pFindReplace->replaceText());
}

//==============================================================================

void EditorFindReplaceWidget::updateHeight()
{
    // Update our layout
    // Note: we shouldn't have to do this, but if the user opens a read-only and
    //       shows ourselves, then the find-related widgets will be too 'low'.
    //       This is because the replace-related widgets get hidden/disabled as
    //       expected, but the layout doesn't get updated before we fix our
    //       height, so we update our layout here and in all cases...

    mGui->layout->update();

    // Update our height

    setFixedHeight(mGui->layout->sizeHint().height());
}

//==============================================================================

void EditorFindReplaceWidget::updateStyleSheet()
{
    // Change the style of our tool buttons

    QColor shadowColor = Core::shadowColor();

    setStyleSheet(QString("QToolButton {"
                          "    border: 0px;"
                          "    border-radius: 3px;"
                          "    padding: 1px;"
                          "}"
                          ""
                          "QToolButton:focus {"
                          "    background: rgba(%1, %2, %3, 0.13);"
                          "    border: 1px solid rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:hover {"
                          "    background: rgba(%1, %2, %3, 0.39);"
                          "}"
                          ""
                          "QToolButton:pressed {"
                          "    background: rgba(%1, %2, %3, 0.79);"
                          "}").arg(QString::number(shadowColor.red()),
                                   QString::number(shadowColor.green()),
                                   QString::number(shadowColor.blue())));
}

//==============================================================================

void EditorFindReplaceWidget::changeEvent(QEvent *pEvent)
{
    // Check whether the palette has changed and if so then update our style
    // sheet

    if (pEvent->type() == QEvent::PaletteChange)
        updateStyleSheet();
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

        Core::Widget::keyPressEvent(pEvent);
}

//==============================================================================

void EditorFindReplaceWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::Widget::resizeEvent(pEvent);

    // Update our height

    updateHeight();
}

//==============================================================================

void EditorFindReplaceWidget::on_findPreviousButton_clicked()
{
    // Let people know that we want to find the previous occurrence of the text

    emit findPreviousRequested();
}

//==============================================================================

void EditorFindReplaceWidget::on_findNextButton_clicked()
{
    // Let people know that we want to find the next occurrence of the text

    emit findNextRequested();
}

//==============================================================================

void EditorFindReplaceWidget::on_replaceButton_clicked()
{
    // Let people know that we want to replace the current text

    emit replaceRequested();
}

//==============================================================================

void EditorFindReplaceWidget::on_replaceAndFindButton_clicked()
{
    // Let people know that we want to replace the current text and the find the
    // next occurence of the text

    emit replaceAndFindRequested();
}

//==============================================================================

void EditorFindReplaceWidget::on_replaceAllButton_clicked()
{
    // Let people know that we want to replace all the occurences of the text

    emit replaceAllRequested();
}

//==============================================================================

void EditorFindReplaceWidget::searchOptionChanged()
{
    // Update the icon used for the leading position of our find widget

    int nbOfOptions =  mCaseSensitiveAction->isChecked()
                      +mWholeWordsOnlyAction->isChecked()
                      +mRegularExpressionAction->isChecked();

    if (nbOfOptions) {
        static QIcon caseSensitiveIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/casesensitively.png");
        static QIcon wholeWordsOnlyIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/wholewords.png");
        static QIcon regularExpressionIcon = QIcon(":/qtCreator/src/plugins/coreplugin/find/images/regexp.png");

        static int IconSize = 16;
        static int IconWidth = 6;

        QPixmap dropDownPixmap = QPixmap(nbOfOptions*IconWidth-mRegularExpressionAction->isChecked(),
                                         IconSize);
        // Note: -mRegularExpressionAction->isChecked(), because
        //       regularExpressionIcon is effectively one pixel narrower than
        //       caseSensitiveIcon and wholeWordsOnlyIcon...

        dropDownPixmap.fill(Qt::transparent);

        QPainter dropDownPixmapPainter(&dropDownPixmap);

        int left = -IconWidth;

        if (mCaseSensitiveAction->isChecked()) {
            caseSensitiveIcon.paint(&dropDownPixmapPainter, left, 0, IconSize, IconSize);

            left += IconWidth;
        }

        if (mWholeWordsOnlyAction->isChecked()) {
            wholeWordsOnlyIcon.paint(&dropDownPixmapPainter, left, 0, IconSize, IconSize);

            left += IconWidth;
        }

        if (mRegularExpressionAction->isChecked())
            regularExpressionIcon.paint(&dropDownPixmapPainter, left-1, 0, IconSize, IconSize);

        mDropDownAction->setIcon(dropDownPixmap);
    } else {
        // No options are set, so use our default icon

        static QIcon defaultIcon = QIcon(":/qtCreator/src/plugins/coreplugin/images/magnifier.png");

        mDropDownAction->setIcon(defaultIcon);
    }
}

//==============================================================================

void EditorFindReplaceWidget::updateClearFindTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our find widget
    // contains some text

    if (pText.isEmpty())
        mGui->findEdit->removeAction(mClearFindTextAction);
    else
        mGui->findEdit->addAction(mClearFindTextAction, QLineEdit::TrailingPosition);

    // Let people know whether we can find/replace

    emit canFindReplace(!findText().isEmpty());
}

//==============================================================================

void EditorFindReplaceWidget::updateClearReplaceTextAction(const QString &pText)
{
    // Show/hide our clear text action, based on whether our replace widget
    // contains some text

    if (pText.isEmpty())
        mGui->replaceEdit->removeAction(mClearReplaceTextAction);
    else
        mGui->replaceEdit->addAction(mClearReplaceTextAction, QLineEdit::TrailingPosition);
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
